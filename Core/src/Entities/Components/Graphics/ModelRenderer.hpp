#ifndef MODELRENDERER_HPP
#define MODELRENDERER_HPP

#include "Entities/ComponentManager.hpp"
#include "Graphics/ResourceManager.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct ModelRenderer final : Component {
        Core::Graphics::ModelHandle model;

        ModelRenderer() = default;
        explicit ModelRenderer(const Core::Graphics::ModelHandle model) : model(model) {}
    };
} // namespace Pixf::Core::Entities::Components::Graphics

#endif // MODELRENDERER_HPP
