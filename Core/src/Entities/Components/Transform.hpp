#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Math/Math.hpp"
#include "Serialization/Serializable.hpp"

namespace Pixf::Core::Entities::Components {
    struct PIXF_API Transform final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(Transform)

        Math::Vector3f position;
        Math::Quaternion rotation;
        Math::Vector3f eulerRotation;
        Math::Vector3f scale{1.0F};

        Math::Matrix4f GetMatrix() const;

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            if (editorMode) {
                json["position"] = position.Serialize(true);
                json["rotation"] = eulerRotation.Serialize(true);
                json["scale"] = scale.Serialize(true);
            } else {
                json["position"] = position.Serialize(false);
                json["rotation"] = rotation.Serialize(false);
                json["scale"] = scale.Serialize(false);
            }

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            if (editorMode) {
                position.Deserialize(json.at("position").as_object(), assetManager, true);
                eulerRotation.Deserialize(json.at("rotation").as_object(), assetManager, true);
                scale.Deserialize(json.at("scale").as_object(), assetManager, true);
            } else {
                position.Deserialize(json.at("position").as_object(), assetManager, false);
                rotation.Deserialize(json.at("rotation").as_object(), assetManager, false);
                scale.Deserialize(json.at("scale").as_object(), assetManager, false);
            }
        }
    };
} // namespace Pixf::Core::Entities::Components

#endif // TRANSFORM_HPP
