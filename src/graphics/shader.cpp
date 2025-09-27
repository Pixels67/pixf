#include "shader.h"

#include <glad/glad.h>

#include <iostream>
#include <sstream>
#include <string>

#include "core/string_utils.h"
#include "file/file_io.h"
#include "gtc/type_ptr.hpp"

constexpr int POS_LAYOUT_INDEX = 0;
constexpr int UV_LAYOUT_INDEX = 1;

constexpr auto DEFAULT_SHADER = R"(#version 330 core
@VERTEX
@pos in vec3 aPos;
@uv in vec2 aUv;
out vec2 uv;
out vec4 color;
uniform vec4 uColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   uv = aUv;
   color = uColor;
}
@FRAGMENT
uniform sampler2D tex;
in vec2 uv;
in vec4 color;
out vec4 FragColor;
void main()
{
    FragColor = texture(tex, uv) * color;
})";

namespace engine::graphics {
Shader::Shader(const std::string& vert_shader_src,
               const std::string& frag_shader_src)
    : m_id_(glCreateProgram()),
      m_vert_shader_src_(vert_shader_src),
      m_frag_shader_src_(frag_shader_src) {
  const unsigned int vert_shader = CreateVertShader(vert_shader_src);
  const unsigned int frag_shader = CreateFragShader(frag_shader_src);

  glAttachShader(m_id_, vert_shader);
  glAttachShader(m_id_, frag_shader);

  glLinkProgram(m_id_);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  int success = 0;
  glGetProgramiv(m_id_, GL_LINK_STATUS, &success);

  if (success == 0) {
    char info_log[512];
    glGetProgramInfoLog(m_id_, 512, nullptr, info_log);
    std::cerr << "Shader linking failed!\n" << info_log << '\n';
    exit(EXIT_FAILURE);
  }
}

void Shader::Unbind() { glUseProgram(0); }

Shader::Shader(const Shader& other) {
  *this = std::move(Shader(other.m_vert_shader_src_, other.m_frag_shader_src_));
}

Shader& Shader::operator=(const Shader& other) {
  if (this == &other) {
    return *this;
  }

  if (m_id_ != 0) {
    this->~Shader();
  }

  *this = std::move(Shader(other.m_vert_shader_src_, other.m_frag_shader_src_));
  return *this;
}

Shader::Shader(Shader&& other) noexcept {
  if (this == &other) {
    return;
  }

  this->m_id_ = other.m_id_;
  other.m_id_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (m_id_ != 0) {
    this->~Shader();
  }

  this->m_id_ = other.m_id_;
  other.m_id_ = 0;

  return *this;
}

Shader::~Shader() { glDeleteProgram(m_id_); }

Shader Shader::CreateShader() { return CreateShader(DEFAULT_SHADER); }

Shader Shader::CreateShader(const std::string& src) {
  auto [vertSrc, fragSrc] = ParseShader(src);
  return Shader(vertSrc, fragSrc);
}

void Shader::Bind() const { glUseProgram(m_id_); }

void Shader::SetUniform(const std::string& name,
                        const std::initializer_list<int> values) const {
  Bind();

  const auto* const it = values.begin();
  switch (values.size()) {
    case 1:
      glUniform1i(glGetUniformLocation(m_id_, name.c_str()), *it);
      break;
    case 2:
      glUniform2i(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1));
      break;
    case 3:
      glUniform3i(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1),
                  *(it + 2));
      break;
    case 4:
      glUniform4i(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1),
                  *(it + 2), *(it + 3));
      break;
    default:
      break;
  }

  Unbind();
}

void Shader::SetUniform(
    const std::string& name,
    const std::initializer_list<unsigned int> values) const {
  Bind();

  const auto* const it = values.begin();
  switch (values.size()) {
    case 1:
      glUniform1ui(glGetUniformLocation(m_id_, name.c_str()), *it);
      break;
    case 2:
      glUniform2ui(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1));
      break;
    case 3:
      glUniform3ui(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1),
                   *(it + 2));
      break;
    case 4:
      glUniform4ui(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1),
                   *(it + 2), *(it + 3));
      break;
    default:
      break;
  }

  Unbind();
}

void Shader::SetUniform(const std::string& name,
                        const std::initializer_list<float> values) const {
  Bind();

  const auto* const it = values.begin();
  switch (values.size()) {
    case 1:
      glUniform1f(glGetUniformLocation(m_id_, name.c_str()), *it);
      break;
    case 2:
      glUniform2f(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1));
      break;
    case 3:
      glUniform3f(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1),
                  *(it + 2));
      break;
    case 4:
      glUniform4f(glGetUniformLocation(m_id_, name.c_str()), *it, *(it + 1),
                  *(it + 2), *(it + 3));
      break;
    default:
      break;
  }

  Unbind();
}

void Shader::SetUniform(const std::string& name, glm::mat4 matrix) const {
  Bind();

  const unsigned int matrix_loc = glGetUniformLocation(m_id_, name.c_str());
  glUniformMatrix4fv(static_cast<GLint>(matrix_loc), 1, GL_FALSE,
                     value_ptr(matrix));

  Unbind();
}

Shader Shader::LoadFromFile(const std::string& path) {
  const std::string str = file::ReadFile(path);
  auto [vertSrc, fragSrc] = ParseShader(str);
  return Shader(vertSrc, fragSrc);
}

Shader::ShaderSources Shader::ParseShader(const std::string& source) {
  std::string str = source;
  str = core::ReplaceAll(
      str, "@pos",
      "layout (location = " + std::to_string(POS_LAYOUT_INDEX) + ")");
  str = core::ReplaceAll(
      str, "@uv",
      "layout (location = " + std::to_string(UV_LAYOUT_INDEX) + ")");

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

unsigned int Shader::CreateVertShader(const std::string& src) {
  const unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
  const char* c_str = src.c_str();
  glShaderSource(shader, 1, &c_str, nullptr);
  glCompileShader(shader);

  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == 0) {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    std::cerr << "Vertex shader compilation failed!\n" << info_log << '\n';
    exit(EXIT_FAILURE);
  }

  return shader;
}

unsigned int Shader::CreateFragShader(const std::string& src) {
  const unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* c_str = src.c_str();
  glShaderSource(shader, 1, &c_str, nullptr);
  glCompileShader(shader);

  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == 0) {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    std::cerr << "Fragment shader compilation failed!\n" << info_log << '\n';
    exit(EXIT_FAILURE);
  }

  return shader;
}
}  // namespace engine::graphics
