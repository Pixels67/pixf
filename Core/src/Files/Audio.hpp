#ifndef PIXF_AUDIO_HPP
#define PIXF_AUDIO_HPP

#include "Audio/Ma/Clip.hpp"
#include "Common.hpp"

namespace Pixf::Core::Files {
    PIXF_API Audio::Ma::Clip LoadAudioFile(const std::string &filepath);
} // namespace Pixf::Core::Files

#endif // PIXF_AUDIO_HPP
