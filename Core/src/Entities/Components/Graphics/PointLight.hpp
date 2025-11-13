#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Serialization/Serializable.hpp"
#include "Serialization/Serialization.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PIXF_API PointLight final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(PointLight)

        vec3 position = vec3(0.0F);
        vec3 color = vec3(1.0F);
        float intensity = 1.0F;

        float linearFalloff = 0.1F;
        float quadraticFalloff = 0.03F;

        PointLight() = default;
        PointLight(const vec3 position, const vec3 color, const float intensity, const float linearFalloff,
                   const float quadraticFalloff) :
            position(position), color(color), intensity(intensity), linearFalloff(linearFalloff),
            quadraticFalloff(quadraticFalloff) {}

        Json::object Serialize() override {
            Json::object json;

            json["position"] = Serialization::SerializeVec3(position);
            json["color"] = Serialization::SerializeColorRgb(color);
            json["intensity"] = intensity;
            json["linearFalloff"] = linearFalloff;
            json["quadraticFalloff"] = quadraticFalloff;

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            position = Serialization::DeserializeVec3(json.at("position").as_object());
            color = Serialization::DeserializeColorRgb(json.at("color").as_object());
            intensity = json.at("intensity").to_number<float>();
            linearFalloff = json.at("linearFalloff").to_number<float>();
            quadraticFalloff = json.at("quadraticFalloff").to_number<float>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // POINTLIGHT_HPP
