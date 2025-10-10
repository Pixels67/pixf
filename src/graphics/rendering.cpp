#include "rendering.h"

#include <memory>

#include "camera.h"
#include "entity_manager.h"
#include "lighting/point_light.h"
#include "mesh.h"

namespace pixf::graphics {
void RenderSystem::OnInit(EntityManager &entity_manager) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!entity_manager.IsSingletonRegistered<Camera>()) {
    return;
  }

  const glm::vec2 viewport_size = entity_manager.GetSingleton<Camera>()->viewport_size;
  glViewport(0, 0, static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));
}

void RenderSystem::OnUpdate(EntityManager &entity_manager, const double delta_time) {
  if (!entity_manager.IsSingletonRegistered<Camera>()) {
    return;
  }

  const auto resource_manager = entity_manager.GetSingleton<ResourceManager>();
  std::vector<lighting::PointLight> point_lights;
  for (const auto &[entity, light] : entity_manager.Query<lighting::PointLight>()) {
    point_lights.push_back(*light);
  }

  const auto cam = entity_manager.GetSingleton<Camera>();

  glClearColor(cam->bg_color.r, cam->bg_color.g, cam->bg_color.b, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (entity_manager.IsComponentRegistered<MeshRenderer>()) {
    for (const auto &[entity, component] : entity_manager.Query<MeshRenderer>()) {
      auto transform = core::Transform{};
      if (auto transform_ptr = entity_manager.GetEntityComponent<core::Transform>(entity);
          transform_ptr != nullptr) {
        transform = *transform_ptr;
      }

      MeshRenderConfig render_config{
          cam->transform,      point_lights, cam->GetProjectionMatrix(),
          component->material, transform,    cam->bg_color,
      };

      const std::shared_ptr<const Mesh> mesh = resource_manager->GetMesh(component->mesh);
      mesh->Render(render_config, *resource_manager);
    }
  } else if (entity_manager.IsComponentRegistered<ModelRenderer>()) {
    for (const auto &[entity, component] : entity_manager.Query<ModelRenderer>()) {
      auto transform = core::Transform{};
      if (auto transform_ptr = entity_manager.GetEntityComponent<core::Transform>(entity);
          transform_ptr != nullptr) {
        transform = *transform_ptr;
      }

      ModelRenderConfig render_config{
          component->materials,       cam->transform, point_lights,
          cam->GetProjectionMatrix(), transform,      cam->bg_color,
      };

      const Model model = component->model;
      component->model.Render(render_config, *resource_manager);
    }
  }
}
}  // namespace pixf::graphics
