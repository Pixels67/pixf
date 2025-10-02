#include "shader_manager.h"

#include <optional>
#include <string>
#include <unordered_map>

namespace pixf::graphics {
ShaderHandle ShaderManager::CreateShader() {
  const unsigned int id = GenShaderId().value();
  shaders.emplace(id, std::move(gl::Shader()));
  return {id};
}

ShaderHandle ShaderManager::CreateShader(const std::string& src) {
  const unsigned int id = GenShaderId().value();
  shaders.emplace(id, src);
  return {id};
}

const gl::Shader& ShaderManager::GetShader(const ShaderHandle handle) const {
  return shaders.at(handle.id);
}

void ShaderManager::Bind(const ShaderHandle handle) const { shaders.at(handle.id).Bind(); }

void ShaderManager::Unbind() { gl::Shader::Unbind(); }

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const std::initializer_list<int> values) const {
  shaders.at(handle.id).SetUniform(name, values);
}

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const std::initializer_list<unsigned int> values) const {
  shaders.at(handle.id).SetUniform(name, values);
}

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const std::initializer_list<float> values) const {
  shaders.at(handle.id).SetUniform(name, values);
}

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const glm::mat4& matrix) const {
  shaders.at(handle.id).SetUniform(name, matrix);
}

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const glm::vec3 value) const {
  shaders.at(handle.id).SetUniform(name, value);
}
void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const glm::vec4 value) const {
  shaders.at(handle.id).SetUniform(name, value);
}

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const std::vector<float>& values) const {
  shaders.at(handle.id).SetUniform(name, values);
}

void ShaderManager::SetUniform(const ShaderHandle handle, const std::string& name,
                               const std::vector<glm::vec3>& values) const {
  shaders.at(handle.id).SetUniform(name, values);
}

std::optional<unsigned int> ShaderManager::GenShaderId() const {
  for (unsigned int i = 1; i < std::numeric_limits<unsigned int>::max(); i++) {
    if (shaders.find(i) == shaders.end()) {
      return i;
    }
  }

  // I have absolutely no idea how you would possibly get here...
  return {};
}
}  // namespace pixf::graphics