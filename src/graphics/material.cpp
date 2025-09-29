#include "material.h"

namespace pixf::graphics {
Material::Material(const Texture& texture) : texture_(texture), default_shader_(true) {
  shader_ = Shader(true);
  color_ = glm::vec4(1.0F);
  
  
}

Material::Material(const Material& other) {
  if (this == &other) { return;
}

  shader_ = other.shader_;
  color_ = other.color_;
  default_shader_ = other.default_shader_;
  if (other.texture_.has_value()) {
    texture_ = other.texture_.value();
  } else {
    texture_ = std::nullopt;
  }
}

Material& Material::operator=(const Material& other) {
  if (this == &other) { return *this;
}

  shader_ = other.shader_;
  color_ = other.color_;
  default_shader_ = other.default_shader_;
  if (other.texture_.has_value()) {
    texture_ = other.texture_.value();
  } else {
    texture_ = std::nullopt;
  }

  return *this;
}

void Material::SetShader(const Shader& shader) {
  this->shader_ = shader;
  default_shader_ = false;
}

void Material::SetColor(const glm::vec4& color) { this->color_ = color; }

void Material::SetTexture(const Texture& texture) {
  this->texture_ = texture;
  if (default_shader_) {
    shader_ = Shader{true};
  }
}

const Shader& Material::GetShader() const { return shader_; }

void Material::Bind() const {
  shader_.SetUniform("uColor", {color_.r, color_.g, color_.b, color_.a});
  shader_.Bind();
  if (texture_.has_value()) {
    texture_.value().Bind(0);
  }
}

void Material::Unbind() {
  Shader::Unbind();
  Texture::Unbind(0);
}
}  // namespace pixf::graphics
