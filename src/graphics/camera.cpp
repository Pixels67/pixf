#include "camera.h"

namespace engine::graphics {
glm::mat4 Camera::GetViewMatrix() const {
  const glm::mat4 t = translate(glm::mat4(1.0F), -position);
  const glm::mat4 r = mat4_cast(inverse(normalize(rotation)));
  return r * t;
}

void Camera::Translate(const glm::vec3 translation) { position += translation; }

void Camera::Rotate(const float delta_angle_degrees, const glm::vec3 axis) {
  const glm::quat delta_rotation =
      angleAxis(glm::radians(delta_angle_degrees), normalize(axis));
  rotation = normalize(delta_rotation * rotation);
}

void Camera::LookAt(const glm::vec3 target, const glm::vec3 up) {
  const glm::vec3 forward = normalize(target - position);
  rotation = quatLookAt(forward, normalize(up));
}
}  // namespace engine::graphics