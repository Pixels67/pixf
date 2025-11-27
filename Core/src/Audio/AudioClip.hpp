#ifndef PIXF_AUDIOCLIP_HPP
#define PIXF_AUDIOCLIP_HPP

#include "Handle.hpp"

namespace Pixf::Core::Audio {
    struct AudioClip {
        ClipHandle m_Clip;
        double volume = 1.0F;
        double pitch = 1.0F;
        double pan = 0.0F;
        bool loop = false;
        bool spatialize = false;
    };
} // namespace Pixf::Core::Audio

#endif // PIXF_AUDIOCLIP_HPP
