#include "AudioManager.hpp"

#include "miniaudio.h"

namespace Pixf::Core::Audio {
    AudioClipError AudioClip::Load(const std::string &path, ma_engine &engine) {
        if (ma_sound_init_from_file(&engine, path.c_str(), 0, nullptr, nullptr, &m_Clip) != MA_SUCCESS) {
            return AudioClipError::FailedToLoad;
        }

        m_IsInitialized = true;
        return AudioClipError::None;
    }

    AudioClip::~AudioClip() {
        if (m_IsInitialized) {
            ma_sound_uninit(&m_Clip);
        }
    }

    AudioManagerError AudioManager::InitAudioManager(AudioManagerConfig config) {
        if (ma_engine_init(nullptr, &m_Engine) != MA_SUCCESS) {
            return AudioManagerError::FailedToInitialize;
        }

        return AudioManagerError::None;
    }

    AudioManager::~AudioManager() { ma_engine_uninit(&m_Engine); }

    AudioImportResult AudioManager::ImportAudioClip(const std::string &path) {
        m_AudioClipCounter++;
        m_AudioClips[m_AudioClipCounter] = AudioClip();
        if (auto result = m_AudioClips[m_AudioClipCounter].Load(path, m_Engine); result != AudioClipError::None) {
            m_AudioClips.erase(m_AudioClipCounter);
            return result;
        }

        return AudioClipHandle(m_AudioClipCounter);
    }

    void AudioManager::SetListener(const ListenerConfig &config) {
        ma_engine_listener_set_position(&m_Engine, 0, config.position.x, config.position.y, config.position.z);
        ma_engine_listener_set_direction(&m_Engine, 0, config.direction.x, config.direction.y, config.direction.z);
    }

    void AudioManager::PlayAudioClip(const AudioClipHandle handle, const AudioPlayConfig &config) {
        if (!m_AudioClips.contains(handle.m_Id)) {
            return;
        }

        auto &clip = m_AudioClips.at(handle.m_Id);

        ma_sound_set_looping(&clip.m_Clip, config.loop);
        ma_sound_set_volume(&clip.m_Clip, config.volume);
        ma_sound_set_pitch(&clip.m_Clip, config.pitch);
        ma_sound_set_pan(&clip.m_Clip, config.pan);

        ma_sound_set_spatialization_enabled(&clip.m_Clip, config.spatialize);
        ma_sound_set_position(&clip.m_Clip, config.position.x, config.position.y, config.position.z);

        ma_sound_start(&clip.m_Clip);
    }

    void AudioManager::DeleteAudioClip(const AudioClipHandle handle) {
        if (m_AudioClips.contains(handle.m_Id)) {
            m_AudioClips.erase(handle.m_Id);
        }
    }
} // namespace Pixf::Core::Audio
