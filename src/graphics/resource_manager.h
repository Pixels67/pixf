#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "entity_manager.h"
#include "gl/texture.h"

namespace pixf::graphics {
struct Vertex;
class Mesh;

struct ShaderHandle {
  unsigned int id = 0;
};

struct TextureHandle {
  unsigned int id = 0;
};

struct MeshHandle {
  unsigned int id = 0;
};

namespace gl {
class Shader;
}
struct ResourceManager final : Component {
  ShaderHandle CreateShader();
  ShaderHandle CreateShader(const std::string& src);

  TextureHandle CreateTexture(const std::string& path,
                              gl::TextureConfig config = {gl::TextureConfig::InterpMode::NEAREST,
                                                          gl::TextureConfig::WrapMode::REPEAT});

  MeshHandle CreateMesh(const std::vector<Vertex>& vertices,
                        const std::vector<unsigned int>& indices);
  MeshHandle CreateMesh(const std::vector<Vertex>& vertices);

  std::shared_ptr<const gl::Shader> GetShader(ShaderHandle handle) const;
  std::shared_ptr<const gl::Texture> GetTexture(TextureHandle handle) const;
  std::shared_ptr<const Mesh> GetMesh(MeshHandle handle) const;

 private:
  std::unordered_map<unsigned int, std::shared_ptr<gl::Shader>> shaders{};
  std::unordered_map<unsigned int, std::shared_ptr<gl::Texture>> textures{};
  std::unordered_map<unsigned int, std::shared_ptr<Mesh>> meshes{};

  std::optional<unsigned int> GenShaderId() const;
  std::optional<unsigned int> GenTextureId() const;
  std::optional<unsigned int> GenMeshId() const;
};
}  // namespace pixf::graphics