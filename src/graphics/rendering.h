#pragma once

#include "entity_manager.h"
#include "mesh.h"
#include "systems_manager.h"

namespace pixf::graphics {
struct Renderable final : Component {
  Mesh mesh;
  Material material;

  Renderable& operator=(const Renderable& other) = default;
};

struct RenderSystem final : System {
  void OnInit(EntityManager& entity_manager) override;
  void OnUpdate(EntityManager& entity_manager, double delta_time) override;
};
}  // namespace pixf::graphics