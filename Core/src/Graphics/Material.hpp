#ifndef FLK_MATERIAL_HPP
#define FLK_MATERIAL_HPP

#include "Pipeline.hpp"
#include "Asset/AssetHandle.hpp"
#include "Math/Math.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct FLK_API Material {
        Asset::AssetHandle<Pipeline> pipeline = Asset::AssetHandle<Pipeline>::FromPath("@PBR");

        Color4u8 color     = Color4u8::White();
        f32      metallic  = 0.25F;
        f32      roughness = 0.75F;

        Asset::AssetHandle<Texture> colorMap     = Asset::AssetHandle<Texture>::FromPath("");
        Asset::AssetHandle<Texture> metallicMap  = Asset::AssetHandle<Texture>::FromPath("");
        Asset::AssetHandle<Texture> roughnessMap = Asset::AssetHandle<Texture>::FromPath("");
    };

    FLK_ARCHIVE(Material, pipeline, color, metallic, roughness, colorMap, metallicMap, roughnessMap)
}

#endif //FLK_MATERIAL_HPP
