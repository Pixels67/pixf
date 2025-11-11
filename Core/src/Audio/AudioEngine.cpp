#include "AudioEngine.hpp"

#include "miniaudio.h"

namespace Pixf::Core::Audio {
    AudioClipError AudioClip::Load(const std::string &path) {
        if (ma_sound_init_from_file(AudioEngine::GetEngine(), path.c_str(), 0, nullptr, nullptr, &m_Clip) !=
            MA_SUCCESS) {
            return AudioClipError::FailedToLoad;
        }

        m_IsInitialized = true;
        return AudioClipError::None;
    }

    void AudioClip::Cleanup() {
        if (!m_IsInitialized)
            return;

        ma_sound_uninit(&m_Clip);
        m_IsInitialized = false;
    }

    ma_sound *AudioClip::GetClip() { return &m_Clip; }

    void AudioClip::Play(const AudioPlayConfig &config) {
        ma_sound_set_looping(GetClip(), config.loop);
        ma_sound_set_volume(GetClip(), config.volume);
        ma_sound_set_pitch(GetClip(), config.pitch);
        ma_sound_set_pan(GetClip(), config.pan);

        ma_sound_set_spatialization_enabled(GetClip(), config.spatialize);
        ma_sound_set_position(GetClip(), config.position.x, config.position.y, config.position.z);

        ma_sound_start(GetClip());
    }

    AudioEngineError AudioEngine::Init(AudioManagerConfig config) {
        if (ma_engine_init(nullptr, &s_Engine) != MA_SUCCESS) {
            return AudioEngineError::FailedToInitialize;
        }

        return AudioEngineError::None;
    }

    void AudioEngine::Terminate() { ma_engine_uninit(&s_Engine); }

    void AudioEngine::SetListener(const ListenerConfig &config) {
        ma_engine_listener_set_position(&s_Engine, 0, config.position.x, config.position.y, config.position.z);
        ma_engine_listener_set_direction(&s_Engine, 0, config.direction.x, config.direction.y, config.direction.z);
    }

    ma_engine *AudioEngine::GetEngine() { return &s_Engine; }
} // namespace Pixf::Core::Audio
