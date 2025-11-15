#include "Transform.hpp"

namespace Pixf::Core::Entities::Components {
    Math::Matrix4f Transform::GetMatrix() const {
        const Math::Quaternion rot = Math::Quaternion::Euler(eulerRotation).Rotate(this->rotation);
        return Math::Matrix4f::Translate(position) * Math::Matrix4f::Rotate(rot) * Math::Matrix4f::Scale(scale);
    }
} // namespace Pixf::Core::Entities::Components
