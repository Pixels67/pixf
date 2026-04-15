#include "Audio.hpp"

#include <soloud_wav.h>
#include <memory>
#include <string>

#include "soloud_error.h"

namespace Flock::FileIo {
    std::optional<Audio::AudioClip> ReadAudioClip(const std::filesystem::path &filePath) {
        Audio::AudioClip clip{};
        clip.data = std::make_unique<SoLoud::Wav>();
        if (clip.data->load(filePath.string().c_str()) != SoLoud::SO_NO_ERROR) {
            return std::nullopt;
        }

        return clip;
    }
}
