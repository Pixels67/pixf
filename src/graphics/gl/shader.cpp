#include "shader.h"

#include <glad/glad.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core/string_utils.h"
#include "file/file_io.h"
#include "graphics/graphics.h"
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

struct Properties {
    vec4 diffuse;
    vec4 specular;
    float gloss;
};

//@VERTEX

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

out FragData frag_data;
out Properties frag_properties;
out mat4x4 view_trans;

void main()
{
    gl_Position = transforms.proj * transforms.view * transforms.model * vec4(aPos, 1.0);

    frag_data.frag_pos = (transforms.view * transforms.model * vec4(aPos, 1.0)).xyz;
    frag_data.uv = aUv;
    frag_data.normal = mat3(transpose(inverse(transforms.view * transforms.model))) * aNormal;

    view_trans = transforms.view;

    frag_properties = properties;
}

//@FRAGMENT

struct PointLight {
    vec3 light_color;
    vec3 light_pos;
    float k_const;
    float k_linear;
    float k_quadratic;
};

in FragData frag_data;
in Properties frag_properties;
in mat4x4 view_trans;

uniform bool has_diffuse_map;
uniform bool has_specular_map;

uniform vec3 ambient_light;
uniform PointLight point_light[128];
uniform int point_light_count;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;

uniform float gloss;

out vec4 FragColor;

float diffuse(vec3 light_dir, vec3 normal) {
    return max(dot(normal, light_dir), 0.0);
}

float specular(vec3 light_dir, vec3 normal, vec3 view_dir, float gloss) {
    vec3 reflect_dir = reflect(-light_dir, normal);
    return pow(max(dot(view_dir, reflect_dir), 0.0), gloss);
}

void main()
{
    vec4 result = vec4(0.0);
    for (int i = 0; i < point_light_count; i++) {
        vec3 light_pos = (view_trans * vec4(point_light[i].light_pos, 1.0)).xyz;
        vec3 norm = normalize(frag_data.normal);
        vec3 light_dir = normalize(light_pos - frag_data.frag_pos);
        vec3 view_dir = normalize(-frag_data.frag_pos);

        float light_dist = length(light_pos - frag_data.frag_pos);
        float light_i = 1.0 / (point_light[i].k_const + point_light[i].k_linear * light_dist + point_light[i].k_quadratic * light_dist * light_dist);
        vec3 light_f = light_i * point_light[i].light_color;

        vec4 col;
        if (has_diffuse_map) {
            col = texture(diffuse_map, frag_data.uv) * frag_properties.diffuse;
        } else {
            col = frag_properties.diffuse;
        }

        vec4 amb = vec4(col.rgb * ambient_light, col.a);
        vec4 diff = vec4(col.rgb * diffuse(light_dir, norm) * light_f, col.a);

        if (has_specular_map) {
            col = texture(specular_map, frag_data.uv) * frag_properties.specular;
        } else {
            col = frag_properties.specular;
        }

        vec4 spec = vec4(col.rgb * light_f * specular(light_dir, norm, view_dir, exp2(clamp(frag_properties.gloss, 0.0, 1.0) * 10.0)), col.a);

        result += amb + diff + spec;
    }

    FragColor += result;
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

  glUniform1i(glGetUniformLocation(id_, "diffuse_map"), DIFFUSE_MAP_ID);
  glUniform1i(glGetUniformLocation(id_, "roughness_map"), ROUGHNESS_MAP_ID);
  glUniform1i(glGetUniformLocation(id_, "metallic_map"), METALLIC_MAP_ID);
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
