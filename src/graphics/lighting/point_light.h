#pragma once

#include <glm.hpp>

#include "entity_manager.h"

namespace pixf::graphics::gl::lighting {
struct PointLight final : Component {
  glm::vec3 position{};
  glm::vec3 color = glm::vec3{1.0F};
  float intensity = 1.0F;
  float linear_falloff = 0.045F;
  float quadratic_falloff = 0.0075F;
};
}  // namespace pixf::graphics::gl::lighting