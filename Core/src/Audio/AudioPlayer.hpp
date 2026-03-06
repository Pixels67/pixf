#ifndef FLK_AUDIOPLAYER_HPP
#define FLK_AUDIOPLAYER_HPP

#include <soloud.h>

#include "AudioClip.hpp"
#include "AudioListener.hpp"
#include "Common.hpp"
#include "Math/Vector.hpp"

namespace Flock::Audio {
    struct FLK_API AudioConfig {
        Vector3f position   = {};
        f32      volume     = 0.5F;
        f32      pitch      = 1.0F;
        f32      pan        = 0.0F;
        bool     looping    = false;
        bool     spatialize = false;
    };

    class FLK_API AudioPlayer {
        std::unique_ptr<SoLoud::Soloud> m_Player = nullptr;

    public:
        static std::optional<AudioPlayer> Create();

        AudioPlayer() = default;
        ~AudioPlayer();

        AudioPlayer(const AudioPlayer &other)     = delete;
        AudioPlayer(AudioPlayer &&other) noexcept = default;

        AudioPlayer &operator=(const AudioPlayer &other)     = delete;
        AudioPlayer &operator=(AudioPlayer &&other) noexcept = default;

        void Clear() const;

        bool Play(AudioClip &clip, AudioConfig config = {}) const;
        bool Stop(const AudioClip &clip) const;

        void SetListener(const AudioListener &listener) const;
    };
}

#endif //FLK_AUDIOPLAYER_HPP
