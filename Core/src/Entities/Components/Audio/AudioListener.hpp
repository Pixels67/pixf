#ifndef AUDIOLISTENER_HPP
#define AUDIOLISTENER_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Audio {
    struct PIXF_API AudioListener final : Component, Serialization::Serializable {
        PIXF_SERIALIZABLE(AudioListener);

        vec3 position;
        vec3 direction;

        Json::object Serialize() override {
            Json::object json;

            json["position"] = Serialization::SerializeVec3(position);
            json["direction"] = Serialization::SerializeVec3(direction);

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            position = Serialization::DeserializeVec3(json.at("position").as_object());
            direction = Serialization::DeserializeVec3(json.at("direction").as_object());
        }
    };
} // namespace Pixf::Core::Entities::Components::Audio

#endif // AUDIOLISTENER_HPP
