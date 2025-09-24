#include "transform.h"

#include "matrix.hpp"
#include "ext/matrix_transform.hpp"
#include "gtc/quaternion.hpp"

namespace Engine::Core {
    glm::mat4 Transform::GetMatrix() const {
        const glm::mat4 t = translate(glm::mat4(1.0f), position);
        const glm::mat4 r = mat4_cast(normalize(rotation));
        const glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
        return t * r * s;
    }

    void Transform::Translate(const glm::vec3 translation) {
        position += translation;
    }

    void Transform::Scale(const glm::vec3 scale) {
        this->scale *= scale;
    }

    void Transform::Rotate(const float deltaAngleDegrees, const glm::vec3 axis) {
        const glm::quat deltaRotation = angleAxis(glm::radians(deltaAngleDegrees), normalize(axis));
        rotation = normalize(deltaRotation * rotation);
    }

    void Transform::LookAt(const glm::vec3 target, const glm::vec3 up) {
        const glm::vec3 forward = normalize(target - position);
        rotation = quatLookAt(normalize(forward), normalize(up));
    }
} // Engine::Core