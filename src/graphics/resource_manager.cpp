#include "resource_manager.h"

#include <optional>
#include <string>
#include <unordered_map>

#include "gl/shader.h"
#include "gl/texture.h"
#include "mesh.h"

namespace pixf::graphics {
ShaderHandle ResourceManager::CreateShader() {
  const unsigned int id = GenShaderId().value();
  shaders.emplace(id, std::make_shared<gl::Shader>(gl::Shader()));
  return {id};
}

ShaderHandle ResourceManager::CreateShader(const std::string& src) {
  const unsigned int id = GenShaderId().value();
  shaders.emplace(id, std::make_shared<gl::Shader>(gl::Shader(src)));
  return {id};
}

TextureHandle ResourceManager::CreateTexture(const std::string& path,
                                             const gl::TextureConfig config) {
  const unsigned int id = GenTextureId().value();
  textures.emplace(id, std::make_shared<gl::Texture>(gl::Texture(path, config)));
  return {id};
}

MeshHandle ResourceManager::CreateMesh(const std::vector<Vertex>& vertices,
                                       const std::vector<unsigned int>& indices) {
  const unsigned int id = GenMeshId().value();
  meshes.emplace(id, std::make_shared<Mesh>(Mesh(vertices, indices)));
  return {id};
}

MeshHandle ResourceManager::CreateMesh(const std::vector<Vertex>& vertices) {
  const unsigned int id = GenMeshId().value();
  meshes.emplace(id, std::make_shared<Mesh>(Mesh(vertices)));
  return {id};
}

std::shared_ptr<const gl::Shader> ResourceManager::GetShader(const ShaderHandle handle) const {
  if (handle.id == 0) {
    return nullptr;
  }

  return shaders.at(handle.id);
}

std::shared_ptr<const gl::Texture> ResourceManager::GetTexture(const TextureHandle handle) const {
  if (handle.id == 0) {
    return nullptr;
  }

  return textures.at(handle.id);
}

std::shared_ptr<const Mesh> ResourceManager::GetMesh(const MeshHandle handle) const {
  if (handle.id == 0) {
    return nullptr;
  }

  return meshes.at(handle.id);
}

std::optional<unsigned int> ResourceManager::GenShaderId() const {
  for (unsigned int i = 1; i < std::numeric_limits<unsigned int>::max(); i++) {
    if (shaders.find(i) == shaders.end()) {
      return i;
    }
  }

  // I have absolutely no idea how you would possibly get here...
  return {};
}

std::optional<unsigned int> ResourceManager::GenTextureId() const {
  for (unsigned int i = 1; i < std::numeric_limits<unsigned int>::max(); i++) {
    if (textures.find(i) == textures.end()) {
      return i;
    }
  }

  // I still have absolutely no idea how you would possibly get here...
  return {};
}

std::optional<unsigned int> ResourceManager::GenMeshId() const {
  for (unsigned int i = 1; i < std::numeric_limits<unsigned int>::max(); i++) {
    if (meshes.find(i) == meshes.end()) {
      return i;
    }
  }

  // HOW!?!?!
  return {};
}
}  // namespace pixf::graphics