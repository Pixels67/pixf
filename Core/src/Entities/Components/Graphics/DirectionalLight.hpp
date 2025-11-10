#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Serialization/Serializable.hpp"
#include "Serialization/Serialization.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct DirectionalLight final : Component, Serialization::Serializable {
        SERIALIZABLE(DirectionalLight)

        vec3 direction = vec3(0.0F, 0.0F, -1.0F);
        vec3 color = vec3(1.0F);
        float intensity = 1.0F;

        DirectionalLight() = default;
        DirectionalLight(const vec3 direction, const vec3 color, const float intensity) :
            direction(direction), color(color), intensity(intensity) {}

        Json::object Serialize() override {
            Json::object json;

            json["direction"] = Serialization::SerializeVec3(direction);
            json["color"] = Serialization::SerializeColorRgb(color);
            json["intensity"] = intensity;

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            direction = Serialization::DeserializeVec3(json.at("direction").as_object());
            color = Serialization::DeserializeColorRgb(json.at("color").as_object());
            intensity = json.at("intensity").to_number<double>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // DIRECTIONALLIGHT_HPP
