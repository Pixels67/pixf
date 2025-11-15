#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <memory>
#include <string>

#include "Assets/AssetManager.hpp"
#include "Error/Result.hpp"
#include "Math/Math.hpp"
#include "Serialization/Serializable.hpp"
#include "miniaudio.h"

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
        Math::Vector3f position;
        float volume = 1.0F;
        float pitch = 1.0F;
        float pan = 0.0F;
        bool loop = false;
        bool spatialize = false;

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            json["position"] = position.Serialize(editorMode);
            json["volume"] = volume;
            json["pitch"] = pitch;
            json["pan"] = pan;
            json["loop"] = loop;
            json["spatialize"] = spatialize;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode = false) override {
            position.Deserialize(json.at("position").as_object(), assetManager, editorMode);
            volume = json.at("volume").to_number<float>();
            pitch = json.at("pitch").to_number<float>();
            pan = json.at("pan").to_number<float>();
            loop = json.at("loop").as_bool();
            spatialize = json.at("spatialize").as_bool();
        }
    };

    struct PIXF_API AudioListenerConfig final : Serialization::Serializable {
        Math::Vector3f position;
        Math::Vector3f direction;

        Serialization::Json::object Serialize(const bool editorMode = false) override {
            Serialization::Json::object json;

            json["position"] = position.Serialize(editorMode);
            json["direction"] = direction.Serialize(editorMode);

            return json;
        }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         const bool editorMode) override {
            position.Deserialize(json.at("position").as_object(), assetManager, editorMode);
            direction.Deserialize(json.at("direction").as_object(), assetManager, editorMode);
        }
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
        static void SetListener(const AudioListenerConfig &config);

        static ma_engine *GetEngine();

    private:
        inline static ma_engine s_Engine = {};
    };

} // namespace Pixf::Core::Audio

#endif // AUDIOMANAGER_HPP
