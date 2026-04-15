#ifndef FLK_SPRITERENDERER_HPP
#define FLK_SPRITERENDERER_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct FLK_API SpriteRenderer {
        std::string spritePath = "";
        Color4u8    color      = Color4u8::White();
    };

    FLK_ARCHIVE(SpriteRenderer, spritePath, color)
}

#endif //FLK_SPRITERENDERER_HPP
