#ifndef FLK_MATERIAL_HPP
#define FLK_MATERIAL_HPP

#include "Pipeline.hpp"
#include "Asset/AssetHandle.hpp"

namespace Flock::Graphics {
    struct FLK_API Material {
        Asset::AssetHandle<Pipeline> pipeline;

        Color4u8 color     = Color4u8::White();
        f32      metallic  = 0.5F;
        f32      roughness = 0.5F;

        std::optional<Asset::AssetHandle<Texture2D> > colorMap     = std::nullopt;
        std::optional<Asset::AssetHandle<Texture2D> > metallicMap  = std::nullopt;
        std::optional<Asset::AssetHandle<Texture2D> > roughnessMap = std::nullopt;
    };
}

#endif //FLK_MATERIAL_HPP
