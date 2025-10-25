#ifndef AMBIENTLIGHT_HPP
#define AMBIENTLIGHT_HPP

#include "Common.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct AmbientLight final : Component {
        vec3 color = vec3(1.0F);
        float intensity = 0.2F;

        AmbientLight() = default;
        AmbientLight(const vec3 color, const float intensity) : color(color), intensity(intensity) {}
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // AMBIENTLIGHT_HPP
