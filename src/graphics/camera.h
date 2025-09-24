#pragma once

#include "matrix.hpp"
#include "vec3.hpp"
#include "gtc/quaternion.hpp"

namespace Engine::Graphics {

struct Camera {
    glm::vec3 position;
    glm::quat rotation;

    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    void Translate(glm::vec3 translation);

    void Rotate(float deltaAngleDegrees, glm::vec3 axis);

    void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0, 1, 0));
};

} // Graphics::Engine
