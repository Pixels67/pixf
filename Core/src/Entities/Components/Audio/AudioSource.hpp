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

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         bool editorMode = false) override {
            try {
                const auto uuid = uuids::string_generator()(json.at("uuid").as_string().c_str());
                const auto result = assetManager.GetAssetPath(uuid);
                if (result.IsError()) {
                    PIXF_LOG_ERROR(std::string("Failed to deserialize AudioSource: Asset UUID ") +
                                   json.at("uuid").as_string().c_str() + " not registered");
                    return;
                }

                const std::string path = result.Unwrap();

                clip = assetManager.ImportAudioClip(path).Unwrap();
                config.Deserialize(json.at("config").as_object(), assetManager);
            } catch (const std::runtime_error &e) {
                PIXF_LOG_ERROR("Failed to deserialize AudioSource: ", std::string("Deserialization error ") + e.what());
            }
        }
    };
} // namespace Pixf::Core::Entities::Components::Audio
#endif // AUDIOSOURCE_HPP
