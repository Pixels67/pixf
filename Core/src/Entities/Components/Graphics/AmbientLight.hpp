#ifndef AMBIENTLIGHT_HPP
#define AMBIENTLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"
#include "Math/Math.hpp"
#include "Serialization/Serializable.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PIXF_API AmbientLight final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(AmbientLight)

        Math::Color3u8 color = Math::Color3u8(255);
        float intensity = 0.2F;

        AmbientLight() = default;
        AmbientLight(const Math::Color3u8 &color, const float intensity) : color(color), intensity(intensity) {}

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            json["color"] = color.Serialize(editorMode);
            json["intensity"] = intensity;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            color.Deserialize(json.at("color").as_object(), assetManager, editorMode);
            intensity = json.at("intensity").to_number<float>();
        }
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // AMBIENTLIGHT_HPP
