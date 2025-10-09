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

  const bool perspective = entity_manager.IsSingletonRegistered<PerspectiveCamera>();
  const bool orthographic = entity_manager.IsSingletonRegistered<OrthographicCamera>();

  if (perspective) {
    const glm::vec2 viewport_size = entity_manager.GetSingleton<PerspectiveCamera>()->viewport_size;
    glViewport(0, 0, static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));
  } else if (orthographic) {
    const glm::vec2 viewport_size =
        entity_manager.GetSingleton<OrthographicCamera>()->viewport_size;
    glViewport(0, 0, static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));
  }
}

void RenderSystem::OnUpdate(EntityManager &entity_manager, const double delta_time) {
  const bool perspective = entity_manager.IsSingletonRegistered<PerspectiveCamera>();
  const bool orthographic = entity_manager.IsSingletonRegistered<OrthographicCamera>();
  const auto resource_manager = entity_manager.GetSingleton<ResourceManager>();
  std::vector<lighting::PointLight> point_lights;
  for (const auto &[entity, light] : entity_manager.Query<lighting::PointLight>()) {
    point_lights.push_back(*light);
  }

  auto bg_color = glm::vec3(0.0F);
  auto camera_transform = CameraTransform{};
  auto proj_matrix = glm::mat4(1.0F);

  if (perspective) {
    const auto cam = entity_manager.GetSingleton<PerspectiveCamera>();
    bg_color = cam->bg_color;
    camera_transform = cam->transform;
    proj_matrix = cam->GetProjectionMatrix();
  } else if (orthographic) {
    const auto cam = entity_manager.GetSingleton<OrthographicCamera>();
    bg_color = cam->bg_color;
    camera_transform = cam->transform;
    proj_matrix = cam->GetProjectionMatrix();
  }

  glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (entity_manager.IsComponentRegistered<MeshRenderer>()) {
    for (const auto &[entity, component] : entity_manager.Query<MeshRenderer>()) {
      auto transform = core::Transform{};
      if (auto transform_ptr = entity_manager.GetEntityComponent<core::Transform>(entity);
          transform_ptr != nullptr) {
        transform = *transform_ptr;
      }

      MeshRenderConfig render_config{
          std::vector{component->material},
          *resource_manager,
          camera_transform,
          point_lights,
          proj_matrix,
          transform,
          bg_color,
      };

      const std::shared_ptr<const Mesh> mesh = resource_manager->GetMesh(component->mesh);
      mesh->Render(render_config);
    }
  } else if (entity_manager.IsComponentRegistered<ModelRenderer>()) {
    for (const auto &[entity, component] : entity_manager.Query<ModelRenderer>()) {
      auto transform = core::Transform{};
      if (auto transform_ptr = entity_manager.GetEntityComponent<core::Transform>(entity);
          transform_ptr != nullptr) {
        transform = *transform_ptr;
      }

      MeshRenderConfig render_config{
          std::vector{component->material},
          *resource_manager,
          camera_transform,
          point_lights,
          proj_matrix,
          transform,
          bg_color,
      };

      const Model model = component->model;
      component->model.Render(render_config);
    }
  }
}
}  // namespace pixf::graphics
