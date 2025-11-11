#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Serialization/Serializable.hpp"
#include "Serialization/Serialization.hpp"

namespace Pixf::Core::Entities::Components {
    struct Transform final : Component, Serialization::Serializable {
        PIXF_SERIALIZABLE(Transform)

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

        Json::object Serialize() override {
            Json::object json;

            json["position"] = Serialization::SerializeVec3(position);
            json["rotation"] = Serialization::SerializeQuat(rotation);
            json["scale"] = Serialization::SerializeVec3(scale);

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            position = Serialization::DeserializeVec3(json.at("position").as_object());
            rotation = Serialization::DeserializeQuat(json.at("rotation").as_object());
            scale = Serialization::DeserializeVec3(json.at("scale").as_object());
        }
    };
} // namespace Pixf::Core::Entities::Components

#endif // TRANSFORM_HPP
