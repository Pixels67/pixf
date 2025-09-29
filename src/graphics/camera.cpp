#include "camera.h"

namespace pixf::graphics {
glm::mat4 CameraTransform::GetViewMatrix() const {
  const glm::mat4 t = translate(glm::mat4(1.0F), -position);
  const glm::mat4 r = mat4_cast(inverse(normalize(rotation)));
  return r * t;
}

void CameraTransform::Translate(const glm::vec3 translation) { position += translation; }

void CameraTransform::Rotate(const float delta_angle_degrees, const glm::vec3 axis) {
  const glm::quat delta_rotation = angleAxis(glm::radians(delta_angle_degrees), normalize(axis));
  rotation = normalize(delta_rotation * rotation);
}

void CameraTransform::LookAt(const glm::vec3 target, const glm::vec3 up) {
  const glm::vec3 forward = normalize(target - position);
  rotation = quatLookAt(forward, normalize(up));
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
  const float aspect = viewport_size.x / viewport_size.y;
  return glm::perspective(glm::radians(fov_y), aspect, near, far);
}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const {
  const float aspect = viewport_size.x / viewport_size.y;
  return glm::ortho(-aspect * size / 2.0F, aspect * size / 2.0F, -0.5F * size, 0.5F * size, near,
                    far);
}
}  // namespace pixf::graphics