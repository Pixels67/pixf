#include "Audio.hpp"

#include <miniaudio.h>

#include "Audio/SampleFormat.hpp"

namespace Pixf::Core::Files {
    Audio::Ma::Clip LoadAudioFile(const std::string &filepath) {
        ma_decoder decoder;
        ma_decoder_init_file(filepath.c_str(), nullptr, &decoder);

        ma_uint64 frameCount = 0;
        ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);

        const ma_uint32 channels = decoder.outputChannels;

        size_t frameSize = 0;
        switch (decoder.outputFormat) {
            case ma_format_f32:
                frameSize = 4 * channels;
                break;
            case ma_format_s32:
                frameSize = 4 * channels;
                break;
            case ma_format_s24:
                frameSize = 3 * channels;
                break;
            case ma_format_s16:
                frameSize = 2 * channels;
                break;
            case ma_format_u8:
                frameSize = 1 * channels;
                break;
            default:
                frameSize = 4 * channels;
                break;
        }

        const Memory::Buffer buffer{static_cast<size_t>(frameCount) * frameSize};
        ma_decoder_read_pcm_frames(&decoder, buffer.Get(), frameCount, nullptr);

        return Audio::Ma::Clip{.buffer = buffer,
                               .frameCount = frameCount,
                               .frameSize = frameSize,
                               .readOffset = 0,
                               .sampleRate = decoder.outputSampleRate,
                               .channels = channels,
                               .sampleFormat = Audio::FromMaFormat(decoder.outputFormat)};
    }
} // namespace Pixf::Core::Files
