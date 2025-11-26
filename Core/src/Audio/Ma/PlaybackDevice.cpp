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

    PlaybackDevice::~PlaybackDevice() {
        ma_device_uninit(m_Device.get());
    }

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

    PlaybackDevice::PlaybackDevice(const PlaybackDeviceConfig &config) : m_Config(config) {}

    void PlaybackDevice::HandleAudioData(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {

    }

    ma_format PlaybackDevice::ToMaFormat(const SampleFormat sampleFormat) {
        ma_format maFmt = ma_format_unknown;

        switch (sampleFormat) {
            case SampleFormat::Default:
                maFmt = ma_format_unknown;
                break;
            case SampleFormat::Float32:
                maFmt = ma_format_f32;
                break;
            case SampleFormat::Int16:
                maFmt = ma_format_s16;
                break;
            case SampleFormat::Int24:
                maFmt = ma_format_s24;
                break;
            case SampleFormat::Int32:
                maFmt = ma_format_s32;
                break;
            case SampleFormat::UInt8:
                maFmt = ma_format_u8;
                break;
        }

        return maFmt;
    }
} // namespace Pixf::Core::Audio::Ma
