#include "material.h"

#include "graphics.h"

namespace pixf::graphics {
void Material::Bind(const ShaderManager& shader_manager) const {
  shader_manager.SetUniform(shader, "properties.diffuse", diffuse);
  shader_manager.SetUniform(shader, "properties.metallic", {metallic});
  shader_manager.SetUniform(shader, "properties.roughness", {roughness});

  shader_manager.SetUniform(shader, "has_diffuse_map",
                            {static_cast<const int>(diffuse_map.has_value())});
  shader_manager.SetUniform(shader, "has_metallic_map",
                            {static_cast<const int>(metallic_map.has_value())});
  shader_manager.SetUniform(shader, "has_roughness_map",
                            {static_cast<const int>(roughness_map.has_value())});

  shader_manager.Bind(shader);

  if (diffuse_map.has_value()) {
    diffuse_map.value().Bind(DIFFUSE_MAP_ID);
  }

  if (roughness_map.has_value()) {
    roughness_map.value().Bind(ROUGHNESS_MAP_ID);
  }

  if (metallic_map.has_value()) {
    metallic_map.value().Bind(METALLIC_MAP_ID);
  }
}

void Material::Unbind() {
  gl::Shader::Unbind();
  gl::Texture::Unbind(DIFFUSE_MAP_ID);
  gl::Texture::Unbind(ROUGHNESS_MAP_ID);
  gl::Texture::Unbind(METALLIC_MAP_ID);
}
}  // namespace pixf::graphics
