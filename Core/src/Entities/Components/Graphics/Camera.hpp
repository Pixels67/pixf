#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Entities/Components/RigidTransform.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    enum class CameraType : uint8_t { Orthographic, Perspective };

    struct Camera final : Component {
        RigidTransform transform;

        float near = 0.1F;
        float far = 100.0F;
        float aspect;

        CameraType type;

        float size = 5.0F;
        float fov = 60.0F;

        mat4 GetViewMatrix() const;
        mat4 GetProjectionMatrix() const;

        Camera() = default;
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // CAMERA_HPP
