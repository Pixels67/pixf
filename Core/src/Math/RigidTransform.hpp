#ifndef FLK_RIGIDTRANSFORM_HPP
#define FLK_RIGIDTRANSFORM_HPP

#include "Common.hpp"
#include "Math.hpp"
#include "Serial/Archive.hpp"

namespace Flock {
    struct FLK_API RigidTransform {
        Vector3f   position    = {};
        Quaternion rotation    = {};
        Vector3f   eulerAngles = {};

        [[nodiscard]] Matrix4f Matrix() const {
            return Matrix4f::Rotate(rotation) * Matrix4f::Rotate(eulerAngles) * Matrix4f::Translate(position);
        }

        [[nodiscard]] Matrix4f ViewMatrix() const {
            return Matrix4f::Translate(-position) * Matrix4f::Rotate(-eulerAngles) * Matrix4f::Rotate(rotation.Inverse());
        }
    };

    FLK_ARCHIVE(RigidTransform, position, rotation, eulerAngles)
}

#endif //FLK_RIGIDTRANSFORM_HPP
