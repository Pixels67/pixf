#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct DirectionalLight final : Component {
        vec3 direction = vec3(0.0F, 0.0F, -1.0F);
        vec3 color = vec3(1.0F);
        float intensity = 1.0F;

        DirectionalLight() = default;
        DirectionalLight(const vec3 direction, const vec3 color, const float intensity) :
            direction(direction), color(color), intensity(intensity) {}
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // DIRECTIONALLIGHT_HPP
