#ifndef FLK_MATERIAL_HPP
#define FLK_MATERIAL_HPP

#include "Math/Math.hpp"

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

    inline auto Reflect(Material &material) {
        return Reflectable{
            "Material",
            std::make_tuple(
                Field{"pipelinePath", &material.pipelinePath},
                Field{"color", &material.color},
                Field{"metallic", &material.metallic},
                Field{"roughness", &material.roughness},
                Field{"colorMapPath", &material.colorMapPath},
                Field{"metallicMapPath", &material.metallicMapPath},
                Field{"roughnessMapPath", &material.roughnessMapPath}
            )
        };
    }
}

#endif //FLK_MATERIAL_HPP
