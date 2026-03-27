#ifndef FLK_SPRITERENDERER_HPP
#define FLK_SPRITERENDERER_HPP

#include <filesystem>

#include "Common.hpp"

namespace Flock {
    struct FLK_API SpriteRenderer {
        std::string spritePath = "";
    };

    inline auto Reflect(SpriteRenderer &renderer) {
        return Reflectable{
            "SpriteRenderer",
            std::make_tuple(
                Field{"spritePath", &renderer.spritePath}
            )
        };
    }
}

#endif //FLK_SPRITERENDERER_HPP
