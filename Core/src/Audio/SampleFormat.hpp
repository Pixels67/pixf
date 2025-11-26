#ifndef PIXF_SAMPLEFORMAT_HPP
#define PIXF_SAMPLEFORMAT_HPP

namespace Pixf::Core::Audio {
    enum class SampleFormat : uint8_t {
        Default = 0,
        Float32,
        Int16,
        Int24,
        Int32,
        UInt8,
    };
}

#endif // PIXF_SAMPLEFORMAT_HPP
