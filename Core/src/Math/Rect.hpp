#ifndef FLK_RECT_HPP
#define FLK_RECT_HPP

#include "Vector.hpp"

namespace Flock {
    template<typename T>
    struct Rect2 {
        Vector2<T> origin;
        Vector2<T> aspect;
    };

    using Rect2u = Rect2<u32>;
    using Rect2i = Rect2<i32>;
    using Rect2f = Rect2<f32>;
    using Rect2d = Rect2<f64>;

    inline auto Reflect(Rect2u &rect) {
        return Reflectable{
            "Rect2u",
            std::make_tuple(
                Field{"origin", &rect.origin},
                Field{"aspect", &rect.aspect}
            )
        };
    }

    inline auto Reflect(Rect2i &rect) {
        return Reflectable{
            "Rect2i",
            std::make_tuple(
                Field{"origin", &rect.origin},
                Field{"aspect", &rect.aspect}
            )
        };
    }

    inline auto Reflect(Rect2f &rect) {
        return Reflectable{
            "Rect2f",
            std::make_tuple(
                Field{"origin", &rect.origin},
                Field{"aspect", &rect.aspect}
            )
        };
    }

    inline auto Reflect(Rect2d &rect) {
        return Reflectable{
            "Rect2d",
            std::make_tuple(
                Field{"origin", &rect.origin},
                Field{"aspect", &rect.aspect}
            )
        };
    }
}

#endif //FLK_RECT_HPP