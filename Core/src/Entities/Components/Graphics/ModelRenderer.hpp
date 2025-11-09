#ifndef MODELRENDERER_HPP
#define MODELRENDERER_HPP

#include "../../../Assets/AssetManager.hpp"
#include "Entities/ComponentManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct ModelRenderer final : Component {
        Core::Graphics::AssetHandle model;

        ModelRenderer() = default;
        explicit ModelRenderer(const Core::Graphics::AssetHandle model) : model(model) {}
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // MODELRENDERER_HPP
