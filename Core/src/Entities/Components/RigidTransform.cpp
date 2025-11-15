#include "RigidTransform.hpp"

namespace Pixf::Core::Entities::Components {
    Math::Matrix4f RigidTransform::GetMatrix() const {
        const Math::Quaternion rot = Math::Quaternion::Euler(eulerRotation).Rotate(this->rotation);
        return Math::Matrix4f::Translate(position) * Math::Matrix4f::Rotate(rot);
    }
} // namespace Pixf::Core::Entities::Components
