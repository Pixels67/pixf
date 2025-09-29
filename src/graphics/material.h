#pragma once

#include <optional>
#include <vec4.hpp>

#include "shader.h"
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

  void SetShader(const Shader& shader);

  void SetColor(const glm::vec4& color);

  void SetTexture(const Texture& texture);

  const Shader& GetShader() const;

  void Bind() const;
  static void Unbind();

 private:
  Shader shader_;
  std::optional<Texture> texture_ = std::nullopt;
  glm::vec4 color_ = glm::vec4(1.0F);
  bool default_shader_ = true;
};
}  // namespace pixf::graphics
