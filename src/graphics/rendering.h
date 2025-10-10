#pragma once

#include "entity_manager.h"
#include "material.h"
#include "model.h"
#include "resource_manager.h"
#include "systems_manager.h"

namespace pixf::graphics {
struct MeshRenderer final : Component {
  MeshHandle mesh = {};
  Material material = {};
};

struct ModelRenderer final : Component {
  Model model = {};
  std::vector<Material> materials = {};
};

struct RenderSystem final : System {
  void OnInit(EntityManager& entity_manager) override;
  void OnUpdate(EntityManager& entity_manager, double delta_time) override;
};
}  // namespace pixf::graphics