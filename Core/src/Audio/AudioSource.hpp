#ifndef FLK_AUDIOSOURCE_HPP
#define FLK_AUDIOSOURCE_HPP

#include "Common.hpp"
#include "Math/Vector.hpp"

namespace Flock::Audio {
    struct FLK_API AudioSource {
        std::string audioClipPath;
        bool        play       = false;
        Vector3f    position   = {};
        f32         volume     = 0.5F;
        f32         pitch      = 1.0F;
        f32         pan        = 0.0F;
        bool        looping    = false;
        bool        spatialize = false;

        void Play() {
            play = true;
        }
    };
}

#endif //FLK_AUDIOSOURCE_HPP
