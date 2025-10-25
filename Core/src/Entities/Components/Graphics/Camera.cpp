#include "Camera.hpp"

#include "Common.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    mat4 Camera::GetViewMatrix() const { return inverse(transform.GetMatrix()); }

    mat4 Camera::GetProjectionMatrix() const {
        if (type == CameraType::Perspective) {
            return perspective(radians(fov), aspect, near, far);
        }

        if (type == CameraType::Orthographic) {
            return ortho(-aspect * size, aspect * size, -size, size, near, far);
        }

        return mat4(1.0F);
    }
} // namespace Pixf::Core::Entities::Components
