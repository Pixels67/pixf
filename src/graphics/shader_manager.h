#pragma once

#include <string>
#include <unordered_map>

#include "entity_manager.h"
#include "shader.h"

struct ShaderHandle {
  unsigned int id = 0;
};

namespace pixf::graphics {
struct ShaderManager final : Component {
  std::unordered_map<unsigned int, Shader> shaders{};

  ShaderHandle CreateShader(bool textured = false);
  ShaderHandle CreateShader(const std::string& src);

  const Shader& GetShader(ShaderHandle handle) const;

  void Bind(ShaderHandle handle) const;

  static void Unbind();

  void SetUniform(ShaderHandle handle, const std::string& name,
                  std::initializer_list<int> values) const;

  void SetUniform(ShaderHandle handle, const std::string& name,
                  std::initializer_list<unsigned int> values) const;

  void SetUniform(ShaderHandle handle, const std::string& name,
                  std::initializer_list<float> values) const;

  void SetUniform(ShaderHandle handle, const std::string& name, const glm::mat4& matrix) const;

 private:
  std::optional<unsigned int> GenShaderId() const;
};
}  // namespace pixf::graphics