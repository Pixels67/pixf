#pragma once

#include <matrix.hpp>
#include <string>

namespace engine::graphics {
class Shader {
  struct ShaderSources {
    std::string vert_src;
    std::string frag_src;
  };

 public:
  Shader(const Shader& other);

  Shader& operator=(const Shader& other);

  Shader(Shader&& other);

  Shader& operator=(Shader&& other);

  ~Shader();

  static Shader CreateShader();

  static Shader CreateShader(const std::string& src);

  static Shader LoadFromFile(const std::string& path);

  void Bind() const;

  static void Unbind();

  void SetUniform(const std::string& name,
                  std::initializer_list<int> values) const;

  void SetUniform(const std::string& name,
                  std::initializer_list<unsigned int> values) const;

  void SetUniform(const std::string& name,
                  std::initializer_list<float> values) const;

  void SetUniform(const std::string& name, glm::mat4 matrix) const;

 private:
  unsigned int m_id_ = 0;
  std::string m_vert_shader_src_;
  std::string m_frag_shader_src_;

  explicit Shader(const std::string& vert_shader_src,
                  const std::string& frag_shader_src);

  static ShaderSources ParseShader(const std::string& source);

  static unsigned int CreateVertShader(const std::string& src);

  static unsigned int CreateFragShader(const std::string& src);
};
}  // namespace engine::graphics
