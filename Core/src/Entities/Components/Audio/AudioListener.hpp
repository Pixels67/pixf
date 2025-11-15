#ifndef AUDIOLISTENER_HPP
#define AUDIOLISTENER_HPP

#include "Audio/AudioEngine.hpp"
#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Audio {
    struct PIXF_API AudioListener final : Component, Serialization::Serializable {
        PIXF_TYPE_INFO(AudioListener);

        Core::Audio::AudioListenerConfig config;

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            return config.Serialize(editorMode);
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            config.Deserialize(json, assetManager, editorMode);
        }
    };
} // namespace Pixf::Core::Entities::Components::Audio

#endif // AUDIOLISTENER_HPP
