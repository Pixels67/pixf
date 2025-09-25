#pragma once

#include "matrix.hpp"
#include "vec3.hpp"
#include "gtc/quaternion.hpp"

namespace Engine::Core {
    struct Transform {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        [[nodiscard]] glm::mat4 GetMatrix() const;

        void Translate(glm::vec3 translation);

        void Scale(glm::vec3 scale);

        void Rotate(float deltaAngleDegrees, glm::vec3 axis);

        void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));
    };
} // namespace Engine::Core