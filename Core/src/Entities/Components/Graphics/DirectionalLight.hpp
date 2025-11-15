#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Math/Math.hpp"
#include "Serialization/Serializable.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PIXF_API DirectionalLight final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(DirectionalLight)

        Math::Vector3f direction;
        Math::Color3u8 color;
        float intensity = 1.0F;

        DirectionalLight() = default;
        DirectionalLight(const Math::Vector3f &direction, const Math::Color3u8 &color, const float intensity) :
            direction(direction), color(color), intensity(intensity) {}

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            json["direction"] = direction.Serialize(editorMode);
            json["color"] = color.Serialize(editorMode);
            json["intensity"] = intensity;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            direction.Deserialize(json.at("direction").as_object(), assetManager, editorMode);
            color.Deserialize(json.at("color").as_object(), assetManager, editorMode);
            intensity = json.at("intensity").to_number<float>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // DIRECTIONALLIGHT_HPP
