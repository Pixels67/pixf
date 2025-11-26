#ifndef PIXF_DEVICE_HPP
#define PIXF_DEVICE_HPP

#include <miniaudio.h>

#include "Audio/SampleFormat.hpp"
#include "Clip.hpp"
#include "Common.hpp"
#include "Error/Error.hpp"

namespace Pixf::Core::Audio::Ma {
    class PIXF_API PlaybackDeviceError final : public Error::Error {
    public:
        using Error::Error;
    };

    struct PIXF_API PlaybackDeviceConfig {
        SampleFormat sampleFormat = SampleFormat::Default;
        std::optional<unsigned int> channels; // std::nullopt for default
        std::optional<unsigned int> sampleRate; // std::nullopt for default
    };

    class PIXF_API PlaybackDevice {
    public:
        static PlaybackDevice Create(PlaybackDeviceConfig config);

        PlaybackDevice(const PlaybackDevice &) = delete;
        PlaybackDevice(PlaybackDevice &&) = default;
        PlaybackDevice &operator=(const PlaybackDevice &) = delete;
        PlaybackDevice &operator=(PlaybackDevice &&) = default;

        ~PlaybackDevice();

        void Start() const;
        void Stop() const;

        void Play(Clip &clip);
        bool IsPlaying() const;

    private:
        PlaybackDeviceConfig m_Config;
        std::unique_ptr<ma_device> m_Device = std::make_unique<ma_device>();

        explicit PlaybackDevice(const PlaybackDeviceConfig &config);

        static void HandleAudioData(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    };
} // namespace Pixf::Core::Audio::Ma

#endif // PIXF_DEVICE_HPP
