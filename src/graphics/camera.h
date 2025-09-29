#pragma once

#include "entity_manager.h"
#include "gtc/quaternion.hpp"
#include "matrix.hpp"
#include "vec3.hpp"

namespace pixf::graphics {

struct CameraTransform {
  glm::vec3 position = glm::vec3(0.0F, 0.0F, 0.0F);
  glm::quat rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);

  [[nodiscard]] glm::mat4 GetViewMatrix() const;

  void Translate(glm::vec3 translation);

  void Rotate(float delta_angle_degrees, glm::vec3 axis);

  void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));
};

struct PerspectiveCamera final : Component {
  CameraTransform transform = CameraTransform{};
  glm::vec3 bg_color = glm::vec3(0.2F);
  glm::vec2 viewport_size;
  float fov_y = 60.0F;
  float near = 0.1F;
  float far = 100.0F;

  glm::mat4 GetProjectionMatrix() const;
};

struct OrthographicCamera final : Component {
  CameraTransform transform = CameraTransform{};
  glm::vec3 bg_color = glm::vec3(0.2F);
  glm::vec2 viewport_size;
  float size = 5.0F;
  float near = 0.1F;
  float far = 100.0F;

  glm::mat4 GetProjectionMatrix() const;
};

}  // namespace pixf::graphics
