#pragma once

#include <optional>
#include <vec4.hpp>

#include "assimp/scene.h"
#include "resource_manager.h"

namespace pixf::graphics {

struct Material {
  std::optional<TextureHandle> diffuse_map = std::nullopt;
  std::optional<TextureHandle> specular_map = std::nullopt;
  glm::vec4 diffuse = glm::vec4(1.0F);
  glm::vec4 specular = glm::vec4(1.0F);
  float gloss = 0.5F;
  ShaderHandle shader{};

  void Bind(const ResourceManager& resource_manager) const;
  static void Unbind();

  static std::vector<Material> LoadFromModel(const std::string& filepath,
                                             ResourceManager& resource_manager);

 private:
  static std::vector<Material> ProcessNode(const aiNode* node, const aiScene* scene,
                                           ResourceManager& resource_manager);
};
}  // namespace pixf::graphics
