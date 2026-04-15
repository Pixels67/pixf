#include "AudioPlayer.hpp"

#include "Audio/AudioClip.hpp"
#include "Audio/AudioListener.hpp"
#include "soloud.h"
#include "soloud_error.h"
#include "soloud_wav.h"

namespace Flock::Audio {
    std::optional<AudioPlayer> AudioPlayer::Create() {
        AudioPlayer player;
        player.m_Player = std::make_unique<SoLoud::Soloud>();
        if (player.m_Player->init() != SoLoud::SO_NO_ERROR) {
            return std::nullopt;
        }

        player.SetListener({});
        return player;
    }

    AudioPlayer::~AudioPlayer() {
        Clear();
    }

    void AudioPlayer::Clear() const {
        if (!m_Player) {
            return;
        }

        m_Player->stopAll();
        m_Player->deinit();
    }

    bool AudioPlayer::Play(AudioClip &clip, AudioConfig config) const {
        if (!m_Player) {
            return false;
        }

        usize handle;
        if (config.spatialize) {
            auto [x, y, z] = config.position;
            handle         = m_Player->play3d(*clip.data, x, y, z);
        } else {
            handle = m_Player->play(*clip.data);
        }

        m_Player->setVolume(handle, config.volume);
        m_Player->setPan(handle, config.pan);
        m_Player->setRelativePlaySpeed(handle, config.pitch);
        m_Player->setLooping(handle, config.looping);

        clip.playbackHandle = handle;
        return true;
    }

    bool AudioPlayer::Configure(const AudioClip &clip, const AudioConfig &config) const {
        if (!m_Player) {
            return false;
        }

        const usize handle = clip.playbackHandle;

        m_Player->setVolume(handle, config.volume);
        m_Player->setPan(handle, config.pan);
        m_Player->setRelativePlaySpeed(handle, config.pitch);
        m_Player->setLooping(handle, config.looping);

        return true;
    }

    bool AudioPlayer::Stop(const AudioClip &clip) const {
        if (!m_Player || clip.playbackHandle == FLK_INVALID) {
            return false;
        }

        m_Player->stop(clip.playbackHandle);

        return true;
    }

    void AudioPlayer::SetListener(const AudioListener &listener) const {
        const Vector3f pos = listener.position;
        const Vector3f dir = Vector3f::Forward() * listener.rotation;
        const Vector3f up  = Vector3f::Up() * listener.rotation;

        m_Player->set3dListenerPosition(pos.x, pos.y, -pos.z);
        m_Player->set3dListenerAt(dir.x, dir.y, -dir.z);
        m_Player->set3dListenerUp(up.x, up.y, -up.z);

        m_Player->update3dAudio();
    }
}
