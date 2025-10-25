#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components {
    struct Transform final : Component {
        vec3 position = vec3(0.0F, 0.0F, 0.0F);
        quat rotation = quat(1.0F, 0.0F, 0.0F, 0.0F);
        vec3 scale = vec3(1.0F, 1.0F, 1.0F);

        void Translate(const vec3 &translation);
        void Rotate(const quat &rotation);
        void RotateAround(const vec3 &axis, float degrees);
        void LookAt(const vec3 &target, const vec3 &up);
        void Scale(const vec3 &scale);

        mat4 GetMatrix() const;

        Transform() = default;
    };
} // namespace Pixf::Core::Entities::Components

#endif // TRANSFORM_HPP
