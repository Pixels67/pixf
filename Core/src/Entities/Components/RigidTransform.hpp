#ifndef PIXF_RIGIDTRANSFORM_HPP
#define PIXF_RIGIDTRANSFORM_HPP

#include "Math/Matrix.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector.hpp"
#include "Serialization/Serialization.hpp"

namespace Pixf::Core::Entities::Components {
    struct RigidTransform {
        Math::Vector3f position;
        Math::Quaternion rotation;

        void Translate(const Math::Vector3f &translation) { position += translation; }

        void Rotate(const Math::Quaternion &rotation) { this->rotation *= rotation; }

        void Rotate(const Math::Vector3f &eulerAngles) { this->rotation *= Math::Quaternion::Euler(eulerAngles); }

        void Rotate(const float angleDegrees, const Math::Vector3f &axis) {
            this->rotation *= Math::Quaternion::AngleAxis(angleDegrees, axis);
        }

        Math::Matrix4f GetMatrix() const {
            return Math::Matrix4f::Rotate(rotation) * Math::Matrix4f::Translate(position);
        }
    };

    template<class Archive>
    void Serialize(Archive &archive, RigidTransform &transform) {
        Serialization::SerializeNvp(archive, "position", transform.position);
        Serialization::SerializeNvp(archive, "rotation", transform.rotation);
    }

    PIXF_SERIALIZE(RigidTransform)
} // namespace Pixf::Core::Entities::Components

#endif // PIXF_RIGIDTRANSFORM_HPP
