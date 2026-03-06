#ifndef FLK_FILEIO_AUDIO_HPP
#define FLK_FILEIO_AUDIO_HPP

#include <filesystem>

#include "Audio/AudioClip.hpp"

namespace Flock::FileIo {
    std::optional<Audio::AudioClip> LoadAudioClip(const std::filesystem::path &filePath);
}

#endif //FLK_FILEIO_AUDIO_HPP