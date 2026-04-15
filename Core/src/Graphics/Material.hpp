#ifndef FLK_MATERIAL_HPP
#define FLK_MATERIAL_HPP

#include "Math/Math.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct FLK_API Material {
        std::string pipelinePath = "@PBR";

        Color4u8 color     = Color4u8::White();
        f32      metallic  = 0.25F;
        f32      roughness = 0.75F;

        std::string colorMapPath;
        std::string metallicMapPath;
        std::string roughnessMapPath;
    };

    FLK_ARCHIVE(Material, pipelinePath, color, metallic, roughness, colorMapPath, metallicMapPath, roughnessMapPath)
}

#endif //FLK_MATERIAL_HPP
