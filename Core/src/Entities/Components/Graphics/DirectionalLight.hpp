#ifndef PIXF_DIRECTIONALLIGHT_HPP
#define PIXF_DIRECTIONALLIGHT_HPP

#include "Math/Color.hpp"
#include "Math/Vector.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct DirectionalLight {
        Math::Vector3f direction;
        Math::Color3u8 color = Math::Color3u8::White();
        float intensity = 1.0;
    };

    PIXF_SERIALIZE(DirectionalLight,
        PIXF_FIELD(direction, DirectionalLight.direction);
        PIXF_FIELD(color, DirectionalLight.color);
        PIXF_FIELD(intensity, DirectionalLight.intensity);
    )
}

#endif // PIXF_DIRECTIONALLIGHT_HPP
