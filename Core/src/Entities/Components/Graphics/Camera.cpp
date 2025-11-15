#include "Camera.hpp"

#include "Common.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    Math::Matrix4f Camera::GetViewMatrix() const { return transform.GetMatrix().Inverse(); }

    Math::Matrix4f Camera::GetProjectionMatrix() const {
        if (type == CameraType::Perspective) {
            return Math::Matrix4f::Perspective(near, far, fov, aspect);
        }

        if (type == CameraType::Orthographic) {
            return Math::Matrix4f::Orthographic(near, far, -size, size, -aspect * size, aspect * size);
        }

        return Math::Matrix4f();
    }
} // namespace Pixf::Core::Entities::Components
