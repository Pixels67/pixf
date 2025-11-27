#ifndef PIXF_SAMPLEFORMAT_HPP
#define PIXF_SAMPLEFORMAT_HPP

#include <miniaudio.h>

namespace Pixf::Core::Audio::Ma {
    enum class SampleFormat : uint8_t {
        Default = 0,
        Float32,
        Int16,
        Int24,
        Int32,
        UInt8,
    };

    ma_format ToMaFormat(const SampleFormat &format);
    SampleFormat FromMaFormat(const ma_format &format);
} // namespace Pixf::Core::Audio::Ma

#endif // PIXF_SAMPLEFORMAT_HPP
