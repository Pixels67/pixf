#ifndef PIXF_RESOURCES_HPP
#define PIXF_RESOURCES_HPP

#include "MaterialStore.hpp"
#include "MeshStore.hpp"
#include "ShaderStore.hpp"
#include "TextureStore.hpp"

namespace Pixf::Core::Graphics {
    struct Resources {
        ShaderStore shaderStore;
        MeshStore meshStore;
        MaterialStore materialStore;
        TextureStore textureStore;
    };
} // namespace Pixf::Core

#endif // PIXF_RESOURCES_HPP
