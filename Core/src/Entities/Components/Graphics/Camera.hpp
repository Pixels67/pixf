#ifndef PIXF_CAMERA_HPP
#define PIXF_CAMERA_HPP

#include "Entities/Components/RigidTransform.hpp"
#include "Math/Matrix.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    enum class CameraType : uint8_t {
        Perspective,
        Orthographic,
    };

    PIXF_SERIALIZE(CameraType,
        uint8_t uint = static_cast<uint8_t>(CameraType);
        PIXF_FIELD(value, uint);
        CameraType = static_cast<Graphics::CameraType>(uint);
    )

    struct Camera {
        RigidTransform transform;
        CameraType type = CameraType::Perspective;
        float fov = 60.0F;
        float scale = 5.0F;
        float aspectRatio = 16.0F / 9.0F;
        float near = 0.1F;
        float far = 1000.0F;

        Math::Matrix4f GetViewMatrix() const {
            auto rotInverse = Math::Matrix4f::Rotate(transform.rotation.Inverse());
            const auto transInverse = Math::Matrix4f::Translate(-transform.position);
            return rotInverse * transInverse;
        }

        Math::Matrix4f GetProjMatrix() const {
            if (type == CameraType::Perspective) {
                return Math::Matrix4f::Perspective(fov, aspectRatio, near, far);
            }

            if (type == CameraType::Orthographic) {
                return Math::Matrix4f::Orthographic(-aspectRatio * scale,
                                                    aspectRatio * scale,
                                                    -scale,
                                                    scale,
                                                    near,
                                                    far);
            }

            return Math::Matrix4f::Identity();
        }
    };

    PIXF_SERIALIZE(Camera,
        PIXF_FIELD(transform, Camera.transform);
        PIXF_FIELD(type, Camera.type);
        PIXF_FIELD(fov, Camera.fov);
        PIXF_FIELD(scale, Camera.scale);
        PIXF_FIELD(aspectRatio, Camera.aspectRatio);
        PIXF_FIELD(near, Camera.near);
        PIXF_FIELD(far, Camera.far);
    )
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // PIXF_CAMERA_HPP
