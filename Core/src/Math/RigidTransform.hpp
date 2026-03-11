#ifndef FLK_RIGIDTRANSFORM_HPP
#define FLK_RIGIDTRANSFORM_HPP

#include "Common.hpp"
#include "Math.hpp"

namespace Flock {
    struct FLK_API RigidTransform {
        Vector3f   position    = {};
        Quaternion rotation    = {};
        Vector3f   eulerAngles = {};

        [[nodiscard]] Matrix4f GetMatrix() const {
            return Matrix4f::Rotate(rotation) * Matrix4f::Rotate(eulerAngles) * Matrix4f::Translate(position);
        }

        [[nodiscard]] Matrix4f GetViewMatrix() const {
            return Matrix4f::Translate(-position) * Matrix4f::Rotate(-eulerAngles) * Matrix4f::Rotate(rotation.Inverse());
        }
    };

    inline auto Reflect(RigidTransform &transform) {
        return Reflectable{
            "RigidTransform",
            std::make_tuple(
                Field("position", &transform.position),
                Field("rotation", &transform.rotation),
                Field("eulerAngles", &transform.eulerAngles)
            )
        };
    }
}

#endif //FLK_RIGIDTRANSFORM_HPP
