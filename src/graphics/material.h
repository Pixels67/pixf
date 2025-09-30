#pragma once

#include <optional>
#include <vec4.hpp>

#include "gl/shader.h"
#include "gl/texture.h"
#include "shader_manager.h"

namespace pixf::graphics {
class Material {
 public:
  Material() = default;

  explicit Material(const gl::Texture& texture);

  Material(const Material& other);
  Material& operator=(const Material& other);

  Material(Material&& other) = delete;
  Material& operator=(Material&& other) = delete;

  void SetShader(ShaderHandle shader);

  void SetDiffuse(const glm::vec4& color);

  void SetAmbient(const glm::vec4& color);

  void SetSpecular(const glm::vec4& color);

  void SetTexture(const gl::Texture& texture);

  ShaderHandle GetShader() const;

  glm::vec4 GetDiffuse() const;

  glm::vec4 GetAmbient() const;

  glm::vec4 GetSpecular() const;

  void Bind(const ShaderManager& shader_manager) const;
  static void Unbind();

 private:
  ShaderHandle shader_{};
  std::optional<gl::Texture> texture_ = std::nullopt;
  glm::vec4 diffuse_ = glm::vec4(1.0F);
  glm::vec4 ambient_ = glm::vec4(0.1F, 0.1F, 0.1F, 1.0F);
  glm::vec4 specular_ = glm::vec4(1.0F);  // TODO: Add specular highlights
};
}  // namespace pixf::graphics
