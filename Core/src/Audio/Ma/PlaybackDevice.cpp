#include "PlaybackDevice.hpp"

namespace Pixf::Core::Audio::Ma {
    PlaybackDevice PlaybackDevice::Create(PlaybackDeviceConfig config) {
        ma_device_config conf = ma_device_config_init(ma_device_type_playback);

        conf.playback.format = ToMaFormat(config.sampleFormat);
        conf.playback.channels = config.channels.value_or(0);
        conf.sampleRate = config.sampleRate.value_or(0);
        conf.dataCallback = HandleAudioData;
        conf.pUserData = nullptr;

        PlaybackDevice device(config);
        if (ma_device_init(nullptr, &conf, device.m_Device.get()) != MA_SUCCESS) {
            throw PlaybackDeviceError("Failed to initialize playback device");
        }

        return device;
    }

    PlaybackDevice::~PlaybackDevice() { ma_device_uninit(m_Device.get()); }

    void PlaybackDevice::Start() const {
        if (ma_device_start(m_Device.get()) != MA_SUCCESS) {
            throw PlaybackDeviceError("Failed to start playback device");
        }
    }

    void PlaybackDevice::Stop() const {
        if (ma_device_stop(m_Device.get()) != MA_SUCCESS) {
            throw PlaybackDeviceError("Failed to stop playback device");
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void PlaybackDevice::Play(Clip &clip) { m_Device->pUserData = &clip; }

    bool PlaybackDevice::IsPlaying() const { return m_Device->pUserData != nullptr; }

    PlaybackDevice::PlaybackDevice(const PlaybackDeviceConfig &config) : m_Config(config) {}

    void PlaybackDevice::HandleAudioData(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
        if (pDevice->pUserData == nullptr) {
            return;
        }

        const auto clip = static_cast<Clip *>(pDevice->pUserData);

        if (clip->readOffset >= clip->frameCount) {
            clip->readOffset = 0;
            pDevice->pUserData = nullptr;
            return;
        }

        ma_audio_buffer audioBuffer;
        const ma_audio_buffer_config bufferConfig =
                ma_audio_buffer_config_init(ToMaFormat(clip->sampleFormat),
                                            clip->channels,
                                            clip->frameCount - clip->readOffset,
                                            clip->buffer.Get(clip->readOffset * clip->frameSize),
                                            nullptr);

        if (ma_audio_buffer_init(&bufferConfig, &audioBuffer) != MA_SUCCESS) {
            PIXF_CORE_LOG_ERROR("Failed to initialize audio buffer");
            return;
        }

        clip->readOffset += frameCount;

        ma_audio_buffer_read_pcm_frames(&audioBuffer, pOutput, frameCount, false);
        ma_audio_buffer_uninit(&audioBuffer);
    }
} // namespace Pixf::Core::Audio::Ma
