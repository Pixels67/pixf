#ifndef AMBIENTLIGHT_HPP
#define AMBIENTLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Serialization/Serializable.hpp"
#include "Serialization/Serialization.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PIXF_API AmbientLight final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(AmbientLight)

        vec3 color = vec3(1.0F);
        float intensity = 0.2F;

        AmbientLight() = default;
        AmbientLight(const vec3 color, const float intensity) : color(color), intensity(intensity) {}

        Json::object Serialize(bool editorMode = false) override {
            Json::object json;

            json["color"] = Serialization::SerializeColorRgb(color);
            json["intensity"] = intensity;

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager, bool editorMode = false) override {
            color = Serialization::DeserializeColorRgb(json.at("color").as_object());
            intensity = json.at("intensity").to_number<float>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // AMBIENTLIGHT_HPP
