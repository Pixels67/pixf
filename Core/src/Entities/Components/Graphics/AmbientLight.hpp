#ifndef PIXF_AMBIENTLIGHT_HPP
#define PIXF_AMBIENTLIGHT_HPP

#include "Math/Color.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct AmbientLight {
        Math::Color3u8 color = Math::Color3u8::White();
        float intensity = 0.2F;
    };

    PIXF_SERIALIZE(AmbientLight,
        PIXF_FIELD(color, AmbientLight.color);
        PIXF_FIELD(intensity, AmbientLight.intensity);
    )
}

#endif // PIXF_AMBIENTLIGHT_HPP
