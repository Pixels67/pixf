#pragma once

#include "gtc/quaternion.hpp"
#include "matrix.hpp"
#include "vec3.hpp"

namespace engine::core {
struct Transform {
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  [[nodiscard]] glm::mat4 GetMatrix() const;

  void Translate(glm::vec3 translation);

  void Scale(glm::vec3 new_scale);

  void Rotate(float delta_angle_degrees, glm::vec3 axis);

  void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));
};
}  // namespace engine::core