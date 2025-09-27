#pragma once

#include "gtc/quaternion.hpp"
#include "matrix.hpp"
#include "vec3.hpp"

namespace engine::graphics {

struct Camera {
  glm::vec3 position;
  glm::quat rotation;

  [[nodiscard]] glm::mat4 GetViewMatrix() const;

  void Translate(glm::vec3 translation);

  void Rotate(float delta_angle_degrees, glm::vec3 axis);

  void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));
};

}  // namespace engine::graphics
