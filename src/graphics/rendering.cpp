#include "rendering.h"

#include <memory>

#include "entity_manager.h"
#include "mesh.h"

namespace pixf::graphics {
void RenderSystem::OnInit(EntityManager &entity_manager) {
  glEnable(GL_DEPTH_TEST);

  const bool perspective = !entity_manager.Query<PerspectiveCamera>().empty();
  const bool orthographic = !entity_manager.Query<OrthographicCamera>().empty();

  if (perspective) {
    const glm::vec2 viewport_size =
        entity_manager.Query<PerspectiveCamera>()[0].component->viewport_size;
    glViewport(0, 0, static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));
  } else if (orthographic) {
    const glm::vec2 viewport_size =
        entity_manager.Query<OrthographicCamera>()[0].component->viewport_size;
    glViewport(0, 0, static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));
  }
}

void RenderSystem::OnUpdate(EntityManager &entity_manager, double delta_time) {
  const bool perspective = !entity_manager.Query<PerspectiveCamera>().empty();
  const bool orthographic = !entity_manager.Query<OrthographicCamera>().empty();

  if (perspective) {
    const auto cam = entity_manager.Query<PerspectiveCamera>()[0].component;

    glClearColor(cam->bg_color.r, cam->bg_color.g, cam->bg_color.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto &[entity, component] : entity_manager.Query<Renderable>()) {
      auto transform = entity_manager.GetEntityComponent<core::Transform>(entity);
      if (transform == nullptr) {
        continue;
      }

      component->mesh.Render(component->material, cam->transform, cam->GetProjectionMatrix(),
                             *transform);
    }
  } else if (orthographic) {
    const auto cam = entity_manager.Query<OrthographicCamera>()[0].component;

    glClearColor(cam->bg_color.r, cam->bg_color.g, cam->bg_color.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto &[entity, component] : entity_manager.Query<Renderable>()) {
      auto transform = entity_manager.GetEntityComponent<core::Transform>(entity);
      if (transform == nullptr) {
        continue;
      }

      component->mesh.Render(component->material, cam->transform, cam->GetProjectionMatrix(),
                             *transform);
    }
  }
}
}  // namespace pixf::graphics
