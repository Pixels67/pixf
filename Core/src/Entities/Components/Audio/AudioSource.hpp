#ifndef AUDIOSOURCE_HPP
#define AUDIOSOURCE_HPP

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Audio/AudioEngine.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Audio {
    struct AudioSource final : Component, Serialization::Serializable {
        SERIALIZABLE(AudioSource)

        Assets::AssetHandle clip;
        Core::Audio::AudioPlayConfig config;

        Json::object Serialize() override {
            Json::object json;

            json["uuid"] = uuids::to_string(clip.GetUuid());
            json["config"] = config.Serialize();

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            const auto uuid = uuids::string_generator()(json.at("uuid").as_string().c_str());
            clip = assetManager.ImportAudioClip(assetManager.GetAssetPath(uuid).value()).Unwrap();

            config.Deserialize(json.at("config").as_object(), assetManager);
        }
    };
} // namespace Pixf::Core::Entities::Components::Audio
#endif // AUDIOSOURCE_HPP
