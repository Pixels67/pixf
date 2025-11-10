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

    Json::object Transform::Serialize() {
        Json::object json;

        json["position"] = Serialization::SerializeVec3(position);
        json["rotation"] = Serialization::SerializeQuat(rotation);
        json["scale"] = Serialization::SerializeVec3(scale);

        return json;
    }

    void Transform::Deserialize(const Json::object &json, Assets::AssetManager &assetManager) {
        position = Serialization::DeserializeVec3(json.at("position").as_object());
        rotation = Serialization::DeserializeQuat(json.at("rotation").as_object());
        scale = Serialization::DeserializeVec3(json.at("scale").as_object());
    }
} // namespace Pixf::Core::Entities::Components
