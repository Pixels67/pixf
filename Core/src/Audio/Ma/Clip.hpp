#ifndef PIXF_CLIP_HPP
#define PIXF_CLIP_HPP

#include "Memory/Buffer.hpp"
#include "SampleFormat.hpp"

namespace Pixf::Core::Audio::Ma {
    struct Clip {
        Memory::Buffer buffer;
        size_t frameCount = 0;
        size_t frameSize = 4;
        size_t readOffset = 0;
        unsigned int sampleRate = 0;
        unsigned int channels = 0;
        SampleFormat sampleFormat = SampleFormat::Default;

        void Clear() {
            buffer.Clear();
            frameCount = 0;
            frameSize = 4;
            readOffset = 0;
            sampleRate = 0;
            channels = 0;
            sampleFormat = SampleFormat::Default;
        }
    };
} // namespace Pixf::Core::Audio

#endif // PIXF_CLIP_HPP
