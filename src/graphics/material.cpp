#include "material.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "graphics.h"

namespace pixf::graphics {
void Material::Bind(const ResourceManager& resource_manager) const {
  resource_manager.GetShader(shader)->SetUniform("properties.diffuse", diffuse);
  resource_manager.GetShader(shader)->SetUniform("properties.specular", specular);
  resource_manager.GetShader(shader)->SetUniform("properties.gloss", {gloss});

  resource_manager.GetShader(shader)->SetUniform("has_diffuse_map",
                                                 {static_cast<const int>(diffuse_map.has_value())});
  resource_manager.GetShader(shader)->SetUniform(
      "has_specular_map", {static_cast<const int>(specular_map.has_value())});

  resource_manager.GetShader(shader)->Bind();

  if (diffuse_map.has_value()) {
    resource_manager.GetTexture(diffuse_map.value())->Bind(DIFFUSE_MAP_ID);
  }

  if (specular_map.has_value()) {
    resource_manager.GetTexture(specular_map.value())->Bind(METALLIC_MAP_ID);
  }
}

void Material::Unbind() {
  gl::Shader::Unbind();
  gl::Texture::Unbind(DIFFUSE_MAP_ID);
  gl::Texture::Unbind(ROUGHNESS_MAP_ID);
  gl::Texture::Unbind(METALLIC_MAP_ID);
}

std::vector<Material> Material::LoadFromModel(const std::string& filepath,
                                              ResourceManager& resource_manager) {
  std::cout << "Importing model materials for: " << filepath << '\n';
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filepath, 0);

  if (scene == nullptr) {
    std::cerr << importer.GetErrorString() << '\n';
    return {};
  }

  auto mats = ProcessNode(scene->mRootNode, scene, resource_manager);
  if (mats.size() == 0) {
    Material mat{};
    mat.shader = resource_manager.CreateShader();
    mats.push_back(mat);
  }

  std::cout << "Finished importing model materials for: " << filepath << '\n';

  return mats;
}

std::vector<Material> Material::ProcessNode(const aiNode* node, const aiScene* scene,
                                            ResourceManager& resource_manager) {
  std::vector<Material> materials{};
  materials.reserve(scene->mNumMaterials);
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    aiString diff_path;
    aiString spec_path;

    Material material{};

    if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &diff_path) == AI_SUCCESS) {
      material.diffuse_map = resource_manager.CreateTexture(diff_path.C_Str());
    }

    if (mat->GetTexture(aiTextureType_SPECULAR, 0, &spec_path) == AI_SUCCESS) {
      material.specular_map = resource_manager.CreateTexture(spec_path.C_Str());
    }

    if (float gloss = 0.0f; mat->Get(AI_MATKEY_SHININESS, gloss) == AI_SUCCESS) {
      material.gloss = gloss;
    }

    material.shader = resource_manager.GetDefaultShader();
    materials.push_back(material);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    auto child_mats = ProcessNode(node->mChildren[i], scene, resource_manager);
    materials.reserve(materials.size() + child_mats.size());
    for (size_t j = 0; j < child_mats.size(); j++) {
      materials.push_back(child_mats[j]);
    }
  }

  return materials;
}
}  // namespace pixf::graphics
