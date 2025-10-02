#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "entity_manager.h"
#include "gl/shader.h"

struct ShaderHandle {
  unsigned int id = 0;
};

namespace pixf::graphics {
struct ShaderManager final : Component {
  std::unordered_map<unsigned int, gl::Shader> shaders{};

  ShaderHandle CreateShader();
  ShaderHandle CreateShader(const std::string& src);

  const gl::Shader& GetShader(ShaderHandle handle) const;

  void Bind(ShaderHandle handle) const;

  static void Unbind();

  void SetUniform(ShaderHandle handle, const std::string& name,
                  std::initializer_list<int> values) const;

  void SetUniform(ShaderHandle handle, const std::string& name,
                  std::initializer_list<unsigned int> values) const;

  void SetUniform(ShaderHandle handle, const std::string& name,
                  std::initializer_list<float> values) const;

  void SetUniform(ShaderHandle handle, const std::string& name, const glm::mat4& matrix) const;

  void SetUniform(ShaderHandle handle, const std::string& name, glm::vec3 value) const;

  void SetUniform(ShaderHandle handle, const std::string& name, glm::vec4 value) const;

  void SetUniform(ShaderHandle handle, const std::string& name,
                  const std::vector<float>& values) const;

  void SetUniform(ShaderHandle handle, const std::string& name,
                  const std::vector<glm::vec3>& values) const;

 private:
  std::optional<unsigned int> GenShaderId() const;
};
}  // namespace pixf::graphics