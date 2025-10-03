#pragma once

#include <optional>
#include <vec4.hpp>

#include "gl/shader.h"
#include "resource_manager.h"

namespace pixf::graphics {
struct Material {
  std::optional<TextureHandle> diffuse_map = std::nullopt;
  std::optional<TextureHandle> roughness_map = std::nullopt;
  std::optional<TextureHandle> metallic_map = std::nullopt;
  glm::vec4 diffuse = glm::vec4(1.0F);
  float roughness = 0.5F;
  float metallic = 0.5F;
  ShaderHandle shader{};

  void Bind(const ResourceManager& resource_manager) const;
  static void Unbind();
};
}  // namespace pixf::graphics
