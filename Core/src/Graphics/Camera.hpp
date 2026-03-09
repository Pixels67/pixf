#ifndef FLK_CAMERA_HPP
#define FLK_CAMERA_HPP

#include "Common.hpp"
#include "Math/Math.hpp"

namespace Flock::Graphics {
    enum class Projection {
        Orthographic,
        Perspective,
    };

    struct FLK_API Camera {
        Vector3f   position   = {};
        Quaternion rotation   = {};
        Projection projection = Projection::Orthographic;
        f32        size       = 1.0F;
        f32        fovY       = 60.0F;
        f32        nearZ      = 0.1F;
        f32        farZ       = 1000.0F;

        Matrix4f GetViewMatrix() const {
            return Matrix4f::Translate(-position) * Matrix4f::Rotate(rotation.Inverse());
        }

        Matrix4f GetProjMatrix(f32 aspectRatio) const {
            if (projection == Projection::Orthographic) {
                return Matrix4f::Orthographic(-aspectRatio * size, aspectRatio * size, -size, size, nearZ, farZ);
            }

            return Matrix4f::Perspective(fovY, aspectRatio, nearZ, farZ);
        }
    };

    inline auto Reflect(Camera &light) {
        return Reflectable{
            "Camera",
            std::make_tuple(
                Field{"position", &light.position},
                Field{"rotation", &light.rotation},
                Field{"projection", &light.projection},
                Field{"size", &light.size},
                Field{"fovY", &light.fovY},
                Field{"nearZ", &light.nearZ},
                Field{"farZ", &light.farZ}
            )
        };
    }
}

#endif //FLK_CAMERA_HPP
