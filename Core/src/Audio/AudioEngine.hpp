#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <memory>
#include <string>

#include "Error/Result.hpp"
#include "Serialization/Serializable.hpp"
#include "Serialization/Serialization.hpp"
#include "miniaudio.h"
#include "Assets/AssetManager.hpp"

namespace Pixf::Core::Audio {
    class AudioEngine;
    enum class AudioEngineError : uint8_t {
        None = 0,
        FailedToInitialize,
    };

    enum class AudioClipError : uint8_t {
        None = 0,
        FailedToLoad,
    };

    struct AudioManagerConfig {
        // Empty for now
    };

    struct AudioPlayConfig final : Serialization::Serializable {
        vec3 position = vec3(0.0F);
        float volume = 1.0F;
        float pitch = 1.0F;
        float pan = 0.0F;
        bool loop = false;
        bool spatialize = false;

        Json::object Serialize() override {
            Json::object json;

            json["position"] = Serialization::SerializeVec3(position);
            json["volume"] = volume;
            json["pitch"] = pitch;
            json["pan"] = pan;
            json["loop"] = loop;
            json["spatialize"] = spatialize;

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            position = Serialization::DeserializeVec3(json.at("position").as_object());
            volume = json.at("volume").to_number<float>();
            pitch = json.at("pitch").to_number<float>();
            pan = json.at("pan").to_number<float>();
            loop = json.at("loop").as_bool();
            spatialize = json.at("spatialize").as_bool();
        }
    };

    struct PIXF_API ListenerConfig {
        vec3 position = vec3(0.0F);
        vec3 direction = vec3(0.0F);
    };

    struct PIXF_API AudioClip {
        AudioClip() = default;

        AudioClipError Load(const std::string &path);
        void Cleanup();

        ma_sound *GetClip();

        void Play(const AudioPlayConfig &config = {});

    private:
        ma_sound m_Clip;
        bool m_IsInitialized = false;
    };

    using AudioImportResult = Error::Result<AudioClip, AudioClipError>;

    class PIXF_API AudioEngine {
    public:
        static AudioEngineError Init(AudioManagerConfig config);
        static void Terminate();
        static void SetListener(const ListenerConfig &config);

        static ma_engine *GetEngine();

    private:
        inline static ma_engine s_Engine = {};
    };

} // namespace Pixf::Core::Audio

#endif // AUDIOMANAGER_HPP
