#include "camera.h"

namespace Engine::Graphics {
    glm::mat4 Camera::GetViewMatrix() const {
        const glm::mat4 t = translate(glm::mat4(1.0f), -position);
        const glm::mat4 r = mat4_cast(inverse(normalize(rotation)));
        return r * t;
    }

    void Camera::Translate(const glm::vec3 translation) {
        position += translation;
    }

    void Camera::Rotate(const float deltaAngleDegrees, const glm::vec3 axis) {
        const glm::quat deltaRotation = angleAxis(glm::radians(deltaAngleDegrees), normalize(axis));
        rotation = normalize(deltaRotation * rotation);
    }

    void Camera::LookAt(const glm::vec3 target, const glm::vec3 up) {
        const glm::vec3 forward = normalize(target - position);
        rotation = quatLookAt(forward, normalize(up));
    }
} // Graphics::Engine