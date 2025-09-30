#include "transform.h"

#include "ext/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "matrix.hpp"

namespace pixf::core {
glm::mat4 Transform::GetMatrix() const {
  const glm::mat4 t = translate(glm::mat4(1.0F), position);
  const glm::mat4 r = mat4_cast(normalize(rotation));
  const glm::mat4 s = glm::scale(glm::mat4(1.0F), scale);
  return t * r * s;
}

void Transform::Translate(const glm::vec3 translation) { position += translation; }

void Transform::Scale(const glm::vec3 new_scale) { scale *= new_scale; }

void Transform::Rotate(const float delta_angle_degrees, const glm::vec3 axis) {
  const glm::quat deltaRotation = angleAxis(glm::radians(delta_angle_degrees), normalize(axis));
  rotation = normalize(deltaRotation * rotation);
}

void Transform::LookAt(const glm::vec3 target, const glm::vec3 up) {
  const glm::vec3 forward = normalize(target - position);
  rotation = quatLookAtLH(normalize(forward), normalize(up));
}
}  // namespace pixf::core