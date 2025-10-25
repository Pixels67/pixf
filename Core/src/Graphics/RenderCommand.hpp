#ifndef RENDERCOMMAND_HPP
#define RENDERCOMMAND_HPP

#include <vector>

#include "../Common.hpp"
#include "Entities/Components/Graphics/AmbientLight.hpp"
#include "Entities/Components/Graphics/DirectionalLight.hpp"
#include "Entities/Components/Graphics/PointLight.hpp"
#include "ResourceManager.hpp"

namespace Pixf::Core::Graphics {
    struct RenderCommand {
        Entities::Components::Graphics::AmbientLight ambientLight;
        std::vector<Entities::Components::Graphics::DirectionalLight> directionalLights;
        std::vector<Entities::Components::Graphics::PointLight> pointLights;
        MeshHandle mesh;
        MaterialHandle material;
        mat4 model = mat4(1.0F);
        mat4 view = mat4(1.0F);
        mat4 projection = mat4(1.0F);
    };
} // namespace Pixf::Core::Graphics
#endif // RENDERCOMMAND_HPP
