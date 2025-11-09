#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Error/Result.hpp"
#include "miniaudio.h"

namespace Pixf::Core::Audio {
    class AudioManager;
    enum class AudioManagerError : uint8_t {
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

    struct AudioPlayConfig {
        vec3 position = vec3(0.0F);
        float volume = 1.0F;
        float pitch = 1.0F;
        float pan = 0.0F;
        bool loop = false;
        bool spatialize = false;
    };

    struct ListenerConfig {
        vec3 position = vec3(0.0F);
        vec3 direction = vec3(0.0F);
    };

    struct AudioClip {
        friend class AudioManager;
        AudioClip() = default;

    private:
        AudioClipError Load(const std::string &path, ma_engine &engine);
        void Cleanup();

        ma_sound m_Clip;
        bool m_IsInitialized = false;
    };

    struct AudioClipHandle {
        friend class AudioManager;

        AudioClipHandle() = default;

    private:
        unsigned int m_Id = 0;

        explicit AudioClipHandle(const unsigned int id) : m_Id(id) {}
    };

    using AudioImportResult = Error::Result<AudioClipHandle, AudioClipError>;

    class AudioManager {
    public:
        AudioManager() = default;
        AudioManagerError InitAudioManager(AudioManagerConfig config);

        AudioManager(const AudioManager &) = delete;
        AudioManager(AudioManager &&) = delete;
        AudioManager &operator=(const AudioManager &) = delete;
        AudioManager &operator=(AudioManager &&) = delete;

        ~AudioManager();

        AudioImportResult ImportAudioClip(const std::string &path);

        void SetListener(const ListenerConfig &config);

        void PlayAudioClip(AudioClipHandle handle, const AudioPlayConfig &config = {});

        void DeleteAudioClip(AudioClipHandle handle);

    private:
        ma_engine m_Engine;
        std::unordered_map<unsigned int, AudioClip> m_AudioClips;
        unsigned int m_AudioClipCounter = 0;
    };

} // namespace Pixf::Core::Audio

#endif // AUDIOMANAGER_HPP
