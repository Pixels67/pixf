#include "Transform.hpp"

#include "Common.hpp"
#include "Json/Json.hpp"

namespace Pixf::Core::Entities::Components {
    void Transform::Translate(const vec3 &translation) { position += translation; }

    void Transform::Rotate(const quat &rotation) { this->rotation = normalize(rotation * this->rotation); }

    void Transform::RotateAround(const vec3 &axis, const float degrees) {
        const quat rot = angleAxis(radians(degrees), normalize(axis));
        Rotate(rot);
    }

    void Transform::LookAt(const vec3 &target, const vec3 &up) {
        const vec3 dir = normalize(target - position);
        rotation = quatLookAt(dir, normalize(up));
    }

    void Transform::Scale(const vec3 &scale) { this->scale *= scale; }

    mat4 Transform::GetMatrix() const {
        return translate(mat4(1.0F), position) * mat4_cast(normalize(rotation)) * glm::scale(mat4(1.0F), scale);
    }
} // namespace Pixf::Core::Entities::Components
