#include "rendering.h"

#include <memory>

#include "entity_manager.h"
#include "mesh.h"

namespace pixf::graphics {
void RenderSystem::OnInit(EntityManager &entity_manager) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const bool perspective = entity_manager.SingletonExists<PerspectiveCamera>();
  const bool orthographic = entity_manager.SingletonExists<OrthographicCamera>();

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
  const bool perspective = entity_manager.SingletonExists<PerspectiveCamera>();
  const bool orthographic = entity_manager.SingletonExists<OrthographicCamera>();
  const auto shader_manager = entity_manager.GetSingleton<ShaderManager>();
  std::vector<gl::lighting::PointLight> point_lights;
  for (const auto &[entity, light] : entity_manager.Query<gl::lighting::PointLight>()) {
    point_lights.push_back(*light);
  }

  if (perspective) {
    const auto cam = entity_manager.GetSingleton<PerspectiveCamera>();

    glClearColor(cam->bg_color.r, cam->bg_color.g, cam->bg_color.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto &[entity, component] : entity_manager.Query<Renderable>()) {
      auto transform = entity_manager.GetEntityComponent<core::Transform>(entity);
      if (transform == nullptr) {
        continue;
      }

      component->mesh.Render(component->material, *shader_manager, cam->transform,
                             cam->GetProjectionMatrix(), cam->bg_color, point_lights, *transform);
      transform->Rotate(glm::radians(3000.0F * delta_time),
                        normalize(glm::vec3(0.68F, 1.0F, 0.24F)));
      transform->Rotate(glm::radians(3000.0F * delta_time),
                        normalize(glm::vec3(0.23F, -0.54F, -0.82F)));
      transform->Rotate(glm::radians(3000.0F * delta_time),
                        normalize(glm::vec3(-0.28F, 0.29F, -0.54F)));
    }
  } else if (orthographic) {
    const auto cam = entity_manager.GetSingleton<OrthographicCamera>();

    glClearColor(cam->bg_color.r, cam->bg_color.g, cam->bg_color.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto &[entity, component] : entity_manager.Query<Renderable>()) {
      auto transform = entity_manager.GetEntityComponent<core::Transform>(entity);
      if (transform == nullptr) {
        continue;
      }

      component->mesh.Render(component->material, *shader_manager, cam->transform,
                             cam->GetProjectionMatrix(), cam->bg_color, point_lights, *transform);
    }
  }
}
}  // namespace pixf::graphics
