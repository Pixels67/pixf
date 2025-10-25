#ifndef RIGIDTRANSFORM_HPP
#define RIGIDTRANSFORM_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components {
    struct RigidTransform final : Component {
        vec3 position = vec3(0.0F, 0.0F, 0.0F);
        quat rotation = quat(1.0F, 0.0F, 0.0F, 0.0F);

        void Translate(const vec3 &translation);
        void Rotate(const quat &rotation);
        void RotateAround(const vec3 &axis, float degrees);
        void LookAt(const vec3 &target, const vec3 &up);

        mat4 GetMatrix() const;
    };

} // namespace Pixf::Core::Entities::Components

#endif // RIGIDTRANSFORM_HPP
