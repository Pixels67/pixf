#include "RigidTransform.hpp"

namespace Pixf::Core::Entities::Components {
    void RigidTransform::Translate(const vec3 &translation) { position += translation; }

    void RigidTransform::Rotate(const quat &rotation) { this->rotation = normalize(rotation * this->rotation); }

    void RigidTransform::RotateAround(const vec3 &axis, float degrees) {
        const float angle = radians(degrees);
        const quat rot = angleAxis(angle, normalize(axis));
        Rotate(rot);
    }

    void RigidTransform::LookAt(const vec3 &target, const vec3 &up) {
        const vec3 dir = normalize(target - position);
        rotation = quatLookAt(dir, normalize(up));
    }

    mat4 RigidTransform::GetMatrix() const { return translate(mat4(1.0F), position) * mat4_cast(rotation); }
} // namespace Pixf::Core::Entities::Components
