#ifndef PIXF_CLIP_HPP
#define PIXF_CLIP_HPP

#include "Memory/Buffer.hpp"
#include "Audio/SampleFormat.hpp"

namespace Pixf::Core::Audio::Ma {
    struct Clip {
        Memory::Buffer buffer;
        size_t frameCount = 0;
        size_t frameSize = 4;
        size_t readOffset = 0;
        unsigned int sampleRate = 0;
        unsigned int channels = 0;
        SampleFormat sampleFormat = SampleFormat::Default;
    };
} // namespace Pixf::Core::Audio

#endif // PIXF_CLIP_HPP
