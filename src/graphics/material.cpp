#include "material.h"

namespace pixf::graphics {
Material::Material(const Texture& texture) : texture_(texture) {}

Material::Material(const Material& other) {
  if (this == &other) {
    return;
  }

  shader_ = other.shader_;
  color_ = other.color_;
  if (other.texture_.has_value()) {
    texture_ = other.texture_.value();
  } else {
    texture_ = std::nullopt;
  }
}

Material& Material::operator=(const Material& other) {
  if (this == &other) {
    return *this;
  }

  shader_ = other.shader_;
  color_ = other.color_;
  if (other.texture_.has_value()) {
    texture_ = other.texture_.value();
  } else {
    texture_ = std::nullopt;
  }

  return *this;
}

void Material::SetShader(const ShaderHandle shader) { this->shader_ = shader; }

void Material::SetColor(const glm::vec4& color) { this->color_ = color; }

void Material::SetTexture(const Texture& texture) { this->texture_ = texture; }

ShaderHandle Material::GetShader() const { return shader_; }

void Material::Bind(const ShaderManager& shader_manager) const {
  shader_manager.SetUniform(shader_, "uColor", {color_.r, color_.g, color_.b, color_.a});
  shader_manager.Bind(shader_);
  if (texture_.has_value()) {
    texture_.value().Bind(0);
  }
}

void Material::Unbind() {
  Shader::Unbind();
  Texture::Unbind(0);
}
}  // namespace pixf::graphics
