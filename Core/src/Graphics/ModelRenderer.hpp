#ifndef FLK_MODELRENDERER_HPP
#define FLK_MODELRENDERER_HPP

#include "Common.hpp"

namespace Flock::Graphics {
    struct FLK_API ModelRenderer {
        std::string modelPath;
    };

    inline auto Reflect(ModelRenderer &renderer) {
        return Reflectable{
            "ModelRenderer",
            std::make_tuple(
                Field{"modelPath", &renderer.modelPath}
            )
        };
    }
}

#endif //FLK_MODELRENDERER_HPP
