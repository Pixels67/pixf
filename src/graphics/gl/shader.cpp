#include "shader.h"

#include <glad/glad.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core/string_utils.h"
#include "file/file_io.h"
#include "gtc/type_ptr.hpp"

constexpr int POS_LAYOUT_INDEX = 0;
constexpr int UV_LAYOUT_INDEX = 1;
constexpr int NORM_LAYOUT_INDEX = 2;

constexpr auto DEFAULT_SHADER = R"(#version 330 core

struct FragData {
    vec3 normal;
    vec3 frag_pos;
    vec2 uv;
};

struct LightingData {
    vec3 light_diffuse;
    vec3 light_ambient;
    vec3 light_pos;
};

struct Properties {
    vec4 diffuse;
    float metallic;
    float roughness;
};

@VERTEX

struct Transforms {
    mat4 model;
    mat4 view;
    mat4 proj;
};

@pos in vec3 aPos;
@uv in vec2 aUv;
@norm in vec3 aNormal;

uniform Transforms transforms;
uniform Properties properties;
uniform LightingData lighting_data;

out FragData frag_data;
out LightingData frag_lighting_data;
out Properties frag_properties;

void main()
{
    gl_Position = transforms.proj * transforms.view * transforms.model * vec4(aPos, 1.0);

    frag_data.frag_pos = (transforms.view * transforms.model * vec4(aPos, 1.0)).xyz;
    frag_data.uv = aUv;
    frag_data.normal = mat3(transpose(inverse(transforms.view * transforms.model))) * aNormal;

    frag_lighting_data = lighting_data;
    frag_lighting_data.light_pos = (transforms.view * vec4(lighting_data.light_pos, 1.0)).xyz;

    frag_properties = properties;
}

@FRAGMENT

in FragData frag_data;
in LightingData frag_lighting_data;
in Properties frag_properties;

uniform int has_diffuse_map;
uniform int has_roughness_map;
uniform int has_metallic_map;

out vec4 FragColor;

uniform sampler2D diffuse_map;
uniform sampler2D roughness_map;
uniform sampler2D metallic_map;

float diffuse(vec3 light_dir, vec3 normal) {
    return max(dot(normal, light_dir), 0.0);
}

float specular(vec3 light_dir, vec3 normal, vec3 view_dir, float shininess) {
    vec3 reflect_dir = reflect(-light_dir, normal);
    return pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
}

void main()
{
    vec3 norm = normalize(frag_data.normal);
    vec3 light_dir = normalize(frag_lighting_data.light_pos - frag_data.frag_pos);
    vec3 view_dir = normalize(-frag_data.frag_pos);

    vec4 amb;
    vec4 diff;
    if (has_diffuse_map != 0) {
        vec4 col = texture(diffuse_map, frag_data.uv);
        amb = vec4(frag_lighting_data.light_ambient * col.rgb, col.a);
        diff = vec4(col.rgb * diffuse(light_dir, norm) * frag_lighting_data.light_diffuse, col.a);
    } else {
        amb = vec4(frag_lighting_data.light_ambient * frag_properties.diffuse.rgb, frag_properties.diffuse.a);
        diff = vec4(frag_properties.diffuse.rgb * diffuse(light_dir, norm) * frag_lighting_data.light_diffuse, frag_properties.diffuse.a);
    }

    float shininess;
    if (has_metallic_map != 0) {
        vec4 col = texture(metallic_map, frag_data.uv);
        shininess = pow(2.0, clamp((col.r + col.g + col.b) * 8.0 / 3.0, 0.0, 8.0));
    } else {
        shininess = pow(2.0, clamp(frag_properties.metallic * 8.0, 0.0, 8.0));
    }

    float smoothness;
    if (has_roughness_map != 0) {
        vec4 col = texture(roughness_map, frag_data.uv);
        smoothness = 1.0 - (col.r + col.g + col.b) / 3.0;
    } else {
        smoothness = 1.0 - frag_properties.roughness * 1.0;
    }

    vec4 spec = vec4(smoothness * frag_lighting_data.light_diffuse * specular(light_dir, norm, view_dir, shininess), 1.0);

    vec4 result = amb + diff + spec;
    FragColor = result;
})";

namespace pixf::graphics::gl {
Shader::Shader() { Init(DEFAULT_SHADER); }

Shader::Shader(const std::string& src) { Init(src); }

Shader::Shader(const Shader& other) {
  if (this == &other) {
    return;
  }

  Init(other.src_);
}

Shader& Shader::operator=(const Shader& other) {
  if (this == &other) {
    return *this;
  }

  Init(other.src_);
  return *this;
}

void Shader::Unbind() { glUseProgram(0); }

Shader::Shader(Shader&& other) noexcept {
  if (this == &other) {
    return;
  }

  this->id_ = other.id_;
  this->src_ = std::move(other.src_);
  other.id_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  this->Cleanup();

  this->id_ = other.id_;
  this->src_ = std::move(other.src_);
  other.id_ = 0;

  return *this;
}

Shader::~Shader() { Cleanup(); }

void Shader::Bind() const {
  glUseProgram(id_);

  glUniform1i(glGetUniformLocation(id_, "diffuse_map"), 0);
  glUniform1i(glGetUniformLocation(id_, "roughness_map"), 1);
  glUniform1i(glGetUniformLocation(id_, "metallic_map"), 2);
}

void Shader::SetUniform(const std::string& name, const std::initializer_list<int> values) const {
  Bind();

  const auto* const it = values.begin();
  switch (values.size()) {
    case 1:
      glUniform1i(glGetUniformLocation(id_, name.c_str()), *it);
      break;
    case 2:
      glUniform2i(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1));
      break;
    case 3:
      glUniform3i(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1), *(it + 2));
      break;
    case 4:
      glUniform4i(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1), *(it + 2), *(it + 3));
      break;
    default:
      break;
  }

  Unbind();
}

void Shader::SetUniform(const std::string& name,
                        const std::initializer_list<unsigned int> values) const {
  Bind();

  const auto* const it = values.begin();
  switch (values.size()) {
    case 1:
      glUniform1ui(glGetUniformLocation(id_, name.c_str()), *it);
      break;
    case 2:
      glUniform2ui(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1));
      break;
    case 3:
      glUniform3ui(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1), *(it + 2));
      break;
    case 4:
      glUniform4ui(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1), *(it + 2), *(it + 3));
      break;
    default:
      break;
  }

  Unbind();
}

void Shader::SetUniform(const std::string& name, const std::initializer_list<float> values) const {
  Bind();

  const auto* const it = values.begin();
  switch (values.size()) {
    case 1:
      glUniform1f(glGetUniformLocation(id_, name.c_str()), *it);
      break;
    case 2:
      glUniform2f(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1));
      break;
    case 3:
      glUniform3f(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1), *(it + 2));
      break;
    case 4:
      glUniform4f(glGetUniformLocation(id_, name.c_str()), *it, *(it + 1), *(it + 2), *(it + 3));
      break;
    default:
      break;
  }

  Unbind();
}

void Shader::SetUniform(const std::string& name, glm::mat4 matrix) const {
  Bind();

  const unsigned int matrix_loc = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix4fv(static_cast<GLint>(matrix_loc), 1, GL_FALSE, value_ptr(matrix));

  Unbind();
}

void Shader::SetUniform(const std::string& name, const glm::vec3 value) const {
  Bind();

  glUniform3f(glGetUniformLocation(id_, name.c_str()), value.x, value.y, value.z);

  Unbind();
}

void Shader::SetUniform(const std::string& name, const glm::vec4 value) const {
  Bind();

  glUniform4f(glGetUniformLocation(id_, name.c_str()), value.x, value.y, value.z, value.w);

  Unbind();
}

void Shader::SetUniform(const std::string& name, const std::vector<float>& values) const {
  Bind();

  glUniform1fv(glGetUniformLocation(id_, name.c_str()), values.size(), values.data());

  Unbind();
}

void Shader::SetUniform(const std::string& name, const std::vector<glm::vec3>& values) const {
  Bind();

  std::vector<GLfloat> values_vec;
  for (unsigned int i = 0; i < values.size(); i++) {
    values_vec.push_back(values[i].x);
    values_vec.push_back(values[i].y);
    values_vec.push_back(values[i].z);
  }

  glUniform3fv(glGetUniformLocation(id_, name.c_str()), values_vec.size(), values_vec.data());

  Unbind();
}

Shader Shader::LoadFromFile(const std::string& path) {
  const std::string str = file::ReadFile(path);
  return Shader(str);
}

void Shader::Init(const std::string& src) {
  src_ = src;
  auto [vertSrc, fragSrc] = ParseShader(src);

  id_ = glCreateProgram();

  const unsigned int vert_shader = CreateShader(GL_VERTEX_SHADER, vertSrc);
  const unsigned int frag_shader = CreateShader(GL_FRAGMENT_SHADER, fragSrc);

  glAttachShader(id_, vert_shader);
  glAttachShader(id_, frag_shader);

  glLinkProgram(id_);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  int success = 0;
  glGetProgramiv(id_, GL_LINK_STATUS, &success);

  if (success == 0) {
    char info_log[512];
    glGetProgramInfoLog(id_, 512, nullptr, info_log);
    std::cerr << "Shader linking failed!\n" << info_log << '\n';
    exit(EXIT_FAILURE);
  }
}

void Shader::Cleanup() {
  Unbind();
  if (id_ != 0) {
    glDeleteProgram(id_);
    id_ = 0;
  }
}

Shader::ShaderSources Shader::ParseShader(const std::string& source) {
  std::string str = source;
  str =
      core::ReplaceAll(str, "@pos", "layout (location = " + std::to_string(POS_LAYOUT_INDEX) + ")");
  str = core::ReplaceAll(str, "@uv", "layout (location = " + std::to_string(UV_LAYOUT_INDEX) + ")");
  str = core::ReplaceAll(str, "@norm",
                         "layout (location = " + std::to_string(NORM_LAYOUT_INDEX) + ")");

  ShaderSources sources;
  sources.vert_src = "";
  sources.frag_src = "";

  std::istringstream ss(str);
  std::string line;
  bool ignore = false;
  while (std::getline(ss, line)) {
    if (line.find("@FRAGMENT") != std::string::npos) {
      ignore = true;
      continue;
    }

    if (line.find("@VERTEX") != std::string::npos) {
      ignore = false;
      continue;
    }

    if (!ignore) {
      sources.vert_src += line + '\n';
    }
  }

  ignore = false;
  ss = std::istringstream(str);
  while (std::getline(ss, line)) {
    if (line.find("@FRAGMENT") != std::string::npos) {
      ignore = false;
      continue;
    }

    if (line.find("@VERTEX") != std::string::npos) {
      ignore = true;
      continue;
    }

    if (!ignore) {
      sources.frag_src += line + '\n';
    }
  }

  return sources;
}

unsigned int Shader::CreateShader(const unsigned int type, const std::string& src) {
  if (static_cast<int>(type) != GL_VERTEX_SHADER && static_cast<int>(type) != GL_FRAGMENT_SHADER) {
    return 0;
  }

  const unsigned int shader = glCreateShader(type);
  const char* c_str = src.c_str();
  glShaderSource(shader, 1, &c_str, nullptr);
  glCompileShader(shader);

  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == 0) {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    switch (type) {
      case GL_VERTEX_SHADER:
        std::cerr << "Vertex shader compilation failed!\n" << info_log << '\n';
        break;
      case GL_FRAGMENT_SHADER:
        std::cerr << "Fragment shader compilation failed!\n" << info_log << '\n';
        break;
      default:;
    }

    exit(EXIT_FAILURE);
  }

  return shader;
}
}  // namespace pixf::graphics::gl
