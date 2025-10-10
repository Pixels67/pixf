#include "camera.h"

namespace pixf::graphics {
glm::mat4 CameraTransform::GetViewMatrix() const {
  const glm::mat4 trans = translate(glm::mat4(1.0F), -position);
  const glm::mat4 rot = mat4_cast(inverse(normalize(rotation)));
  return rot * trans;
}

void CameraTransform::Translate(const glm::vec3 translation) { position += translation; }

void CameraTransform::Rotate(const float delta_angle_degrees, const glm::vec3 axis) {
  const glm::quat delta_rotation = angleAxis(glm::radians(delta_angle_degrees), normalize(axis));
  rotation = normalize(delta_rotation * rotation);
}

void CameraTransform::LookAt(const glm::vec3 target, const glm::vec3 up) {
  const glm::vec3 forward = normalize(target - position);
  rotation = quatLookAtLH(forward, normalize(up));
}

glm::mat4 Camera::GetProjectionMatrix() const {
  if (type == CameraType::Perspective) {
    const float aspect = viewport_size.x / viewport_size.y;
    return glm::perspectiveLH_NO(glm::radians(fov_y), aspect, near, far);
  }

  const float aspect = viewport_size.x / viewport_size.y;
  return glm::orthoLH_NO(-aspect * size / 2.0F, aspect * size / 2.0F, -0.5F * size, 0.5F * size,
                         near, far);
}
}  // namespace pixf::graphics