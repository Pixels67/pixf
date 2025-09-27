#include "material.h"

namespace engine::graphics {
void Material::SetColor(const glm::vec4& color) { this->color = color; }

void Material::Bind() const {
  shader.SetUniform("uColor", {color.r, color.g, color.b, color.a});
  shader.Bind();
  texture.Bind(0);
}

void Material::Unbind() {
  Shader::Unbind();
  Texture::Unbind(0);
}
}  // namespace engine::graphics
