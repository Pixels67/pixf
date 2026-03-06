#include "Audio.hpp"

#include <soloud.h>
#include <soloud_wav.h>

namespace Flock::FileIo {
    std::optional<Audio::AudioClip> LoadAudioClip(const std::filesystem::path &filePath) {
        Audio::AudioClip clip{};
        clip.data = std::make_unique<SoLoud::Wav>();
        if (clip.data->load(filePath.string().c_str()) != SoLoud::SO_NO_ERROR) {
            return std::nullopt;
        }

        return clip;
    }
}
