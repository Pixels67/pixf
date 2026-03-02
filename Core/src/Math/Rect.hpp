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
}

#endif //FLK_RECT_HPP