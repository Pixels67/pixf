#ifndef PIXF_AMBIENTLIGHT_HPP
#define PIXF_AMBIENTLIGHT_HPP

#include "Math/Color.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct AmbientLight {
        Math::Color4u8 color = Math::Color4u8::White();
        float intensity = 1.0F;
    };

    PIXF_SERIALIZE(AmbientLight,
        PIXF_FIELD(color, AmbientLight.color);
        PIXF_FIELD(intensity, AmbientLight.intensity);
    )
}

#endif // PIXF_AMBIENTLIGHT_HPP
