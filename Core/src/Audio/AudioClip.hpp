#ifndef FLK_AUDIOCLIP_HPP
#define FLK_AUDIOCLIP_HPP

#include <soloud_wav.h>

#include "Common.hpp"

namespace Flock::Audio {
    struct FLK_API AudioClip {
        std::unique_ptr<SoLoud::Wav> data           = nullptr;
        usize                        playbackHandle = FLK_INVALID;
    };
}

#endif //FLK_AUDIOCLIP_HPP
