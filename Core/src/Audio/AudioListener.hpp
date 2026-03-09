#ifndef FLK_AUDIOLISTENER_HPP
#define FLK_AUDIOLISTENER_HPP

#include "Common.hpp"
#include "Math/Math.hpp"

namespace Flock::Audio {
    struct FLK_API AudioListener {
        Vector3f   position = {};
        Quaternion rotation = {};
    };

    inline auto Reflect(AudioListener &listener) {
        return Reflectable{
            "AudioListener",
            std::make_tuple(
                Field{"position", &listener.position},
                Field{"rotation", &listener.rotation}
            )
        };
    }
}

#endif //FLK_AUDIOLISTENER_HPP
