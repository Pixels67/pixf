#ifndef PIXF_LIGHTINGENVIRONMENT_HPP
#define PIXF_LIGHTINGENVIRONMENT_HPP

#include "Entities/Components/Graphics/AmbientLight.hpp"
#include "Entities/Components/Graphics/DirectionalLight.hpp"
#include "Entities/Components/Graphics/PointLight.hpp"

namespace Pixf::Core::Graphics {
    struct LightingEnvironment {
        Entities::Components::Graphics::AmbientLight ambientLight;
        std::vector<Entities::Components::Graphics::PointLight> pointLights;
        std::vector<Entities::Components::Graphics::DirectionalLight> directionalLights;
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_LIGHTINGENVIRONMENT_HPP
