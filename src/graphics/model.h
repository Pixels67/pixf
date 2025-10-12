#pragma once

#include <string>
#include <vector>

#include "mesh.h"
#include "resource_manager.h"

namespace pixf::graphics {

struct ModelRenderConfig {
  const std::vector<Material>& materials;
  const CameraTransform& camera;
  const std::vector<lighting::PointLight>& point_lights;
  glm::mat4 proj;
  core::Transform transform = {};
  glm::vec3 ambient_light;
};

class Model {
 public:
  Model() = default;
  explicit Model(const std::string& filepath, ResourceManager& resource_manager);
  ~Model();

  void Render(const ModelRenderConfig& render_config,
              const ResourceManager& resource_manager) const;

 private:
  std::vector<MeshHandle> meshes_;

  void ProcessNode(const aiNode* node, const aiScene* scene, ResourceManager& resource_manager);
};

}  // namespace pixf::graphics
