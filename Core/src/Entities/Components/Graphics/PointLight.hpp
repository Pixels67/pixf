#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct PointLight final : Component {
        vec3 position = vec3(0.0F);
        vec3 color = vec3(1.0F);
        float intensity = 1.0F;

        float linearFalloff = 0.1F;
        float quadraticFalloff = 0.03F;

        PointLight() = default;
        PointLight(const vec3 position, const vec3 color, const float intensity, const float linearFalloff,
                   const float quadraticFalloff) :
            position(position), color(color), intensity(intensity), linearFalloff(linearFalloff),
            quadraticFalloff(quadraticFalloff) {}
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // POINTLIGHT_HPP
