#ifndef AUDIOSOURCE_HPP
#define AUDIOSOURCE_HPP

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Audio/AudioEngine.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Audio {
    struct AudioSource final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(AudioSource)

        Assets::AssetHandle clip;
        Core::Audio::AudioPlayConfig config;

        Serialization::Json::object Serialize(bool editorMode = false) override {
            Serialization::Json::object json;

            json["uuid"] = uuids::to_string(clip.GetUuid());
            json["config"] = config.Serialize();

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode = false) override {
            const auto uuid = uuids::string_generator()(json.at("uuid").as_string().c_str());
            const std::string path = assetManager.GetAssetPath(uuid).Unwrap(
                    std::string("Failed to deserialize AudioSource: Asset UUID ") +
                    json.at("uuid").as_string().c_str() + " not registered");

            clip = assetManager.ImportAudioClip(path).Unwrap(
                    std::string("Failed to deserialize AudioSource: Unable to import audio clip ") +
                    json.at("uuid").as_string().c_str());

            config.Deserialize(json.at("config").as_object(), assetManager);
        }
    };
} // namespace Pixf::Core::Entities::Components::Audio
#endif // AUDIOSOURCE_HPP
