#ifndef FLK_RECTTRANSFORM_HPP
#define FLK_RECTTRANSFORM_HPP

#include "Common.hpp"
#include "Math/Rect.hpp"

namespace Flock::Gui {
    struct FLK_API RectTransform {
        Rect2i rect;
    };

    inline auto Reflect(RectTransform &trans) {
        return Reflectable{
            "RectTransform",
            std::make_tuple(
                Field{"rect", &trans.rect}
            )
        };
    }
}

#endif //FLK_RECTTRANSFORM_HPP