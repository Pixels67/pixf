#ifndef FLK_RECT_HPP
#define FLK_RECT_HPP

#include "Vector.hpp"
#include "Serial/Archive.hpp"

namespace Flock {
    template<typename T>
    struct Rect2 {
        Vector2<T> origin = {};
        Vector2<T> aspect = {};

        Rect2() = default;

        Rect2(Vector2<T> origin, Vector2<T> aspect) : origin(origin), aspect(aspect) {
        }

        template<typename U>
        Rect2(Rect2<U> rect) {
            origin = Vector2<T>(rect.origin.x, rect.origin.y);
            aspect = Vector2<T>(rect.aspect.x, rect.aspect.y);
        }
    };

    using Rect2u = Rect2<u32>;
    using Rect2i = Rect2<i32>;
    using Rect2f = Rect2<f32>;
    using Rect2d = Rect2<f64>;

    FLK_ARCHIVE(Rect2u, origin, aspect)
    FLK_ARCHIVE(Rect2i, origin, aspect)
    FLK_ARCHIVE(Rect2f, origin, aspect)
    FLK_ARCHIVE(Rect2d, origin, aspect)
}

#endif //FLK_RECT_HPP
