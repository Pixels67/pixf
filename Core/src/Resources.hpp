#ifndef PIXF_RESOURCES_HPP
#define PIXF_RESOURCES_HPP

#include "Graphics/MaterialStore.hpp"
#include "Graphics/MeshStore.hpp"
#include "Graphics/ShaderStore.hpp"
#include "Graphics/TextureStore.hpp"
#include "Audio/Ma/ClipStore.hpp"

namespace Pixf::Core {
    struct Resources {
        Graphics::ShaderStore shaderStore;
        Graphics::MeshStore meshStore;
        Graphics::MaterialStore materialStore;
        Graphics::TextureStore textureStore;
        Audio::Ma::ClipStore clipStore;
    };
} // namespace Pixf::Core

#endif // PIXF_RESOURCES_HPP
