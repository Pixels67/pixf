#pragma once

#include "matrix.hpp"
#include "vec3.hpp"
#include "gtc/quaternion.hpp"

namespace Engine::Core {
    struct Transform {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        [[nodiscard]] glm::mat4 GetMatrix() const;

        void Translate(glm::vec3 translation);

        void Scale(glm::vec3 scale);

        void Rotate(float deltaAngleDegrees, glm::vec3 axis);

        void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));
    };
} // namespace Engine::Core