#ifndef FLK_TRANSFORM_HPP
#define FLK_TRANSFORM_HPP

#include "Common.hpp"
#include "Quaternion.hpp"
#include "Vector.hpp"

namespace Flock {
    struct FLK_API Transform {
        Vector3f   position;
        Quaternion rotation;
        Vector3f   scale = Vector3f::One();
        Vector3f   eulerAngles;

        Matrix4f GetMatrix() const {
            return Matrix4f::Scale(scale) * Matrix4f::Rotate(rotation) * Matrix4f::Translate(position);
        }
    };
}

#endif //FLK_TRANSFORM_HPP
