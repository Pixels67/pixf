#include "material.h"

#include "graphics.h"

namespace pixf::graphics {
void Material::Bind(const ResourceManager& resource_manager) const {
  resource_manager.GetShader(shader)->SetUniform("properties.diffuse", diffuse);
  resource_manager.GetShader(shader)->SetUniform("properties.metallic", {metallic});
  resource_manager.GetShader(shader)->SetUniform("properties.roughness", {roughness});

  resource_manager.GetShader(shader)->SetUniform("has_diffuse_map",
                                                 {static_cast<const int>(diffuse_map.has_value())});
  resource_manager.GetShader(shader)->SetUniform(
      "has_metallic_map", {static_cast<const int>(metallic_map.has_value())});
  resource_manager.GetShader(shader)->SetUniform(
      "has_roughness_map", {static_cast<const int>(roughness_map.has_value())});

  resource_manager.GetShader(shader)->Bind();

  if (diffuse_map.has_value()) {
    resource_manager.GetTexture(diffuse_map.value())->Bind(DIFFUSE_MAP_ID);
  }

  if (roughness_map.has_value()) {
    resource_manager.GetTexture(roughness_map.value())->Bind(ROUGHNESS_MAP_ID);
  }

  if (metallic_map.has_value()) {
    resource_manager.GetTexture(metallic_map.value())->Bind(METALLIC_MAP_ID);
  }
}

void Material::Unbind() {
  gl::Shader::Unbind();
  gl::Texture::Unbind(DIFFUSE_MAP_ID);
  gl::Texture::Unbind(ROUGHNESS_MAP_ID);
  gl::Texture::Unbind(METALLIC_MAP_ID);
}
}  // namespace pixf::graphics
