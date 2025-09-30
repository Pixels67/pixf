#pragma once

#include <optional>
#include <vec4.hpp>

#include "shader.h"
#include "shader_manager.h"
#include "texture.h"

namespace pixf::graphics {
class Material {
 public:
  Material() = default;

  explicit Material(const Texture& texture);

  Material(const Material& other);
  Material& operator=(const Material& other);

  Material(Material&& other) = delete;
  Material& operator=(Material&& other) = delete;

  void SetShader(ShaderHandle shader);

  void SetColor(const glm::vec4& color);

  void SetTexture(const Texture& texture);

  ShaderHandle GetShader() const;

  void Bind(const ShaderManager& shader_manager) const;
  static void Unbind();

 private:
  ShaderHandle shader_{};
  std::optional<Texture> texture_ = std::nullopt;
  glm::vec4 color_ = glm::vec4(1.0F);
};
}  // namespace pixf::graphics
