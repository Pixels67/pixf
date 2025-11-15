#ifndef RENDERCOMMAND_HPP
#define RENDERCOMMAND_HPP

#include <vector>

#include "Assets/AssetManager.hpp"
#include "Entities/Components/Graphics/AmbientLight.hpp"
#include "Entities/Components/Graphics/DirectionalLight.hpp"
#include "Entities/Components/Graphics/PointLight.hpp"
#include "Math/Math.hpp"

namespace Pixf::Core::Graphics {
    struct RenderCommand {
        Entities::Components::Graphics::AmbientLight ambientLight;
        std::vector<Entities::Components::Graphics::DirectionalLight> directionalLights;
        std::vector<Entities::Components::Graphics::PointLight> pointLights;
        Assets::AssetHandle mesh;
        Assets::AssetHandle material;
        Math::Matrix4f model;
        Math::Matrix4f view;
        Math::Matrix4f projection;
    };
} // namespace Pixf::Core::Graphics
#endif // RENDERCOMMAND_HPP
