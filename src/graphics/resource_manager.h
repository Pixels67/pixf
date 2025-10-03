#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "entity_manager.h"
#include "gl/shader.h"
#include "gl/texture.h"

struct ShaderHandle {
  unsigned int id = 0;
};

struct TextureHandle {
  unsigned int id = 0;
};

namespace pixf::graphics {
struct ResourceManager final : Component {
  ShaderHandle CreateShader();
  ShaderHandle CreateShader(const std::string& src);

  TextureHandle CreateTexture(const std::string& path,
                              gl::TextureConfig config = {gl::TextureConfig::InterpMode::NEAREST,
                                                          gl::TextureConfig::WrapMode::REPEAT});

  std::shared_ptr<const gl::Shader> GetShader(ShaderHandle handle) const;
  std::shared_ptr<const gl::Texture> GetTexture(TextureHandle handle) const;

 private:
  std::unordered_map<unsigned int, std::shared_ptr<gl::Shader>> shaders{};
  std::unordered_map<unsigned int, std::shared_ptr<gl::Texture>> textures{};

  std::optional<unsigned int> GenShaderId() const;
  std::optional<unsigned int> GenTextureId() const;
};
}  // namespace pixf::graphics