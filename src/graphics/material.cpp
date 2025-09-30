#include "material.h"

namespace pixf::graphics {
Material::Material(const gl::Texture& texture) : texture_(texture) {}

Material::Material(const Material& other) {
  if (this == &other) {
    return;
  }

  shader_ = other.shader_;
  diffuse_ = other.diffuse_;
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
  diffuse_ = other.diffuse_;
  if (other.texture_.has_value()) {
    texture_ = other.texture_.value();
  } else {
    texture_ = std::nullopt;
  }

  return *this;
}

void Material::SetShader(const ShaderHandle shader) { this->shader_ = shader; }

void Material::SetDiffuse(const glm::vec4& color) { this->diffuse_ = color; }

void Material::SetAmbient(const glm::vec4& color) { this->ambient_ = color; }

void Material::SetSpecular(const glm::vec4& color) { this->specular_ = color; }

void Material::SetTexture(const gl::Texture& texture) { this->texture_ = texture; }

ShaderHandle Material::GetShader() const { return shader_; }

glm::vec4 Material::GetDiffuse() const { return diffuse_; }

glm::vec4 Material::GetAmbient() const { return ambient_; }

glm::vec4 Material::GetSpecular() const { return specular_; }

void Material::Bind(const ShaderManager& shader_manager) const {
  shader_manager.SetUniform(shader_, "uColor", {diffuse_.r, diffuse_.g, diffuse_.b, diffuse_.a});
  shader_manager.Bind(shader_);
  if (texture_.has_value()) {
    texture_.value().Bind(0);
  }
}

void Material::Unbind() {
  gl::Shader::Unbind();
  gl::Texture::Unbind(0);
}
}  // namespace pixf::graphics
