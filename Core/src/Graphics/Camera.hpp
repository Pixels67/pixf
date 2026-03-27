#ifndef FLK_CAMERA_HPP
#define FLK_CAMERA_HPP

#include "Common.hpp"
#include "Math/Math.hpp"
#include "Math/RigidTransform.hpp"

namespace Flock::Graphics {
    enum class Projection {
        Orthographic,
        Perspective,
    };

    struct FLK_API Camera {
        RigidTransform transform  = {};
        Projection     projection = Projection::Orthographic;
        f32            size       = 5.0F;
        f32            fovY       = 60.0F;
        f32            nearZ      = 0.1F;
        f32            farZ       = 1000.0F;

        [[nodiscard]] Matrix4f GetViewMatrix() const {
            return Matrix4f::Translate(-transform.position) * Matrix4f::Rotate(-transform.eulerAngles) *
                   Matrix4f::Rotate(transform.rotation.Inverse());
        }

        [[nodiscard]] Matrix4f GetProjMatrix(const f32 aspectRatio) const {
            if (projection == Projection::Orthographic) {
                return Matrix4f::Orthographic(-aspectRatio * size, aspectRatio * size, -size, size, nearZ, farZ);
            }

            return Matrix4f::Perspective(fovY, aspectRatio, nearZ, farZ);
        }
    };

    inline auto Reflect(Camera &camera) {
        return Reflectable{
            "Camera",
            std::make_tuple(
                Field{"transform", &camera.transform},
                Field{"projection", &camera.projection},
                Field{"size", &camera.size},
                Field{"fovY", &camera.fovY},
                Field{"nearZ", &camera.nearZ},
                Field{"farZ", &camera.farZ}
            )
        };
    }
}

#endif //FLK_CAMERA_HPP
