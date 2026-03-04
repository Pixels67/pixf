#ifndef FLK_MATERIAL_HPP
#define FLK_MATERIAL_HPP

#include "Pipeline.hpp"
#include "Asset/Handle.hpp"

namespace Flock::Graphics {
    struct FLK_API Material {
        Asset::Handle<Pipeline> pipeline;

        Color4u8 color     = Color4u8::White();
        f32      metallic  = 0.5F;
        f32      roughness = 0.5F;

        std::optional<Asset::Handle<Texture2D> > colorMap     = std::nullopt;
        std::optional<Asset::Handle<Texture2D> > metallicMap  = std::nullopt;
        std::optional<Asset::Handle<Texture2D> > roughnessMap = std::nullopt;
    };
}

#endif //FLK_MATERIAL_HPP
