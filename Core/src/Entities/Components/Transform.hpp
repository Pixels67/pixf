#ifndef PIXF_TRANSFORM_HPP
#define PIXF_TRANSFORM_HPP

#include "Math/Matrix.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector.hpp"

namespace Pixf::Core::Entities::Components {
    struct Transform {
        Math::Vector3f position;
        Math::Quaternion rotation;
        Math::Vector3f scale = Math::Vector3f::One();

        void Translate(const Math::Vector3f &translation) { position += translation; }

        void Rotate(const Math::Quaternion &rotation) { this->rotation *= rotation; }

        void Rotate(const Math::Vector3f &eulerAngles) { this->rotation *= Math::Quaternion::Euler(eulerAngles); }

        void Rotate(const float angleDegrees, const Math::Vector3f &axis) {
            this->rotation *= Math::Quaternion::AngleAxis(angleDegrees, axis);
        }

        void Scale(const Math::Vector3f &scale) { this->scale *= scale; }

        Math::Matrix4f GetMatrix() const {
            return Math::Matrix4f::Rotate(rotation) * Math::Matrix4f::Scale(scale) *
                   Math::Matrix4f::Translate(position);
        }

        template<class Archive>
        static void Serialize(Archive &archive, Transform &transform) {
            archive("position", transform.position);
            archive("rotation", transform.rotation);
            archive("scale", transform.scale);
        }
    };
} // namespace Pixf::Core::Entities::Components

#endif // PIXF_TRANSFORM_HPP
