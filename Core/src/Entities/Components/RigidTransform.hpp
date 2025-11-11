#ifndef RIGIDTRANSFORM_HPP
#define RIGIDTRANSFORM_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Serialization/Serializable.hpp"
#include "Serialization/Serialization.hpp"

namespace Pixf::Core::Entities::Components {
    struct PIXF_API RigidTransform final : Component, Serialization::Serializable {
        PIXF_SERIALIZABLE(RigidTransform)

        vec3 position = vec3(0.0F, 0.0F, 0.0F);
        quat rotation = quat(1.0F, 0.0F, 0.0F, 0.0F);

        void Translate(const vec3 &translation);
        void Rotate(const quat &rotation);
        void RotateAround(const vec3 &axis, float degrees);
        void LookAt(const vec3 &target, const vec3 &up);

        mat4 GetMatrix() const;

        Json::object Serialize() override {
            Json::object json;

            json["position"] = Serialization::SerializeVec3(position);
            json["rotation"] = Serialization::SerializeQuat(rotation);

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            position = Serialization::DeserializeVec3(json.at("position").as_object());
            rotation = Serialization::DeserializeQuat(json.at("rotation").as_object());
        }
    };

} // namespace Pixf::Core::Entities::Components

#endif // RIGIDTRANSFORM_HPP
