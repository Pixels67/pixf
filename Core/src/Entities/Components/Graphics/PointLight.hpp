#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Math/Math.hpp"
#include "Serialization/Serializable.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PIXF_API PointLight final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(PointLight)

        Math::Vector3f position;
        Math::Color3u8 color;

        float intensity = 1.0F;
        float linearFalloff = 0.1F;
        float quadraticFalloff = 0.03F;

        PointLight() = default;
        PointLight(const Math::Vector3f &position, const Math::Color3u8 &color, const float intensity,
                   const float linearFalloff, const float quadraticFalloff) :
            position(position), color(color), intensity(intensity), linearFalloff(linearFalloff),
            quadraticFalloff(quadraticFalloff) {}

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            json["position"] = position.Serialize(editorMode);
            json["color"] = color.Serialize(editorMode);
            json["intensity"] = intensity;
            json["linearFalloff"] = linearFalloff;
            json["quadraticFalloff"] = quadraticFalloff;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            position.Deserialize(json.at("position").as_object(), assetManager, editorMode);
            color.Deserialize(json.at("color").as_object(), assetManager, editorMode);
            intensity = json.at("intensity").to_number<float>();
            linearFalloff = json.at("linearFalloff").to_number<float>();
            quadraticFalloff = json.at("quadraticFalloff").to_number<float>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // POINTLIGHT_HPP
