#pragma once

#include <matrix.hpp>
#include <string>
#include <vector>

namespace pixf::graphics::gl {
class Shader {
  struct ShaderSources {
    std::string vert_src;
    std::string frag_src;
  };

 public:
  explicit Shader(bool textured = false);
  explicit Shader(const std::string& src);

  Shader(const Shader& other);
  Shader& operator=(const Shader& other);

  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  ~Shader();

  static Shader LoadFromFile(const std::string& path);

  void Bind() const;

  static void Unbind();

  void SetUniform(const std::string& name, std::initializer_list<int> values) const;

  void SetUniform(const std::string& name, std::initializer_list<unsigned int> values) const;

  void SetUniform(const std::string& name, std::initializer_list<float> values) const;

  void SetUniform(const std::string& name, glm::mat4 matrix) const;

  void SetUniform(const std::string& name, glm::vec3 value) const;

  void SetUniform(const std::string& name, glm::vec4 value) const;

  void SetUniform(const std::string& name, const std::vector<float>& values) const;

  void SetUniform(const std::string& name, const std::vector<glm::vec3>& values) const;

 private:
  std::string src_;
  unsigned int id_ = 0;

  void Init(const std::string& src);
  void Cleanup();

  static ShaderSources ParseShader(const std::string& source);

  static unsigned int CreateShader(unsigned int type, const std::string& src);
};
}  // namespace pixf::graphics::gl
