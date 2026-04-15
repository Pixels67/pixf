#ifndef FLK_AUDIOLISTENER_HPP
#define FLK_AUDIOLISTENER_HPP

#include "Common.hpp"
#include "Math/Math.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Audio {
    struct FLK_API AudioListener {
        Vector3f   position = {};
        Quaternion rotation = {};
    };

    FLK_ARCHIVE(AudioListener, position, rotation)
}

#endif //FLK_AUDIOLISTENER_HPP
