#pragma once

#include <string>
#include <vector>

#include "assimp/scene.h"
#include "mesh.h"
#include "resource_manager.h"

namespace pixf::graphics {

class Model {
 public:
  Model() = default;
  explicit Model(const std::string& filepath, ResourceManager& resource_manager);
  ~Model();

  void Render(const MeshRenderConfig& render_config) const;

 private:
  std::vector<MeshHandle> meshes_;

  void ProcessNode(const aiNode* node, const aiScene* scene, ResourceManager& resource_manager);
};

}  // namespace pixf::graphics
