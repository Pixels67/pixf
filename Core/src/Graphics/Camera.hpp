#ifndef FLK_CAMERA_HPP
#define FLK_CAMERA_HPP

#include "Common.hpp"
#include "Math/Math.hpp"
#include "Math/RigidTransform.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    enum class Projection {
        Orthographic,
        Perspective,
    };

    struct FLK_API Camera {
        RigidTransform transform  = {.position = {0, 0, -10}};
        Projection     projection = Projection::Orthographic;
        f32            size       = 3.0F;
        f32            fovY       = 60.0F;
        f32            nearZ      = 0.1F;
        f32            farZ       = 1000.0F;

        [[nodiscard]] Matrix4f ViewMatrix() const {
            return Matrix4f::Translate(-transform.position) * Matrix4f::Rotate(-transform.eulerAngles) *
                   Matrix4f::Rotate(transform.rotation.Inverse());
        }

        [[nodiscard]] Matrix4f ProjMatrix(const f32 aspectRatio) const {
            if (projection == Projection::Orthographic) {
                return Matrix4f::Orthographic(-aspectRatio * size, aspectRatio * size, -size, size, nearZ, farZ);
            }

            return Matrix4f::Perspective(fovY, aspectRatio, nearZ, farZ);
        }
    };

    FLK_ARCHIVE(Camera, transform, projection, size, fovY, nearZ, farZ)
}

#endif //FLK_CAMERA_HPP
