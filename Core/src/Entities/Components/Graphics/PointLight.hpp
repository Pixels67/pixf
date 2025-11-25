#ifndef PIXF_POINTLIGHT_HPP
#define PIXF_POINTLIGHT_HPP

#include "Math/Vector.hpp"
#include "Math/Color.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PointLight {
        Math::Vector3f position;
        Math::Color3u8 color = Math::Color3u8::White();
        float intensity = 1.0F;

        float linearFalloff = 0.1F;
        float quadraticFalloff = 0.03F;
    };

    PIXF_SERIALIZE(PointLight,
        PIXF_FIELD(position, PointLight.position);
        PIXF_FIELD(color, PointLight.color);
        PIXF_FIELD(intensity, PointLight.intensity);
        PIXF_FIELD(linearFalloff, PointLight.linearFalloff);
        PIXF_FIELD(quadraticFalloff, PointLight.quadraticFalloff);
    )
}

#endif // PIXF_POINTLIGHT_HPP
