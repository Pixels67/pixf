#ifndef FLK_TIME_HPP
#define FLK_TIME_HPP

#include "Common.hpp"

namespace Flock::Time {
    struct FLK_API Clock {
        f64 time;
        f64 deltaTime;
    };

    inline f64 CurrentTime() {
        return glfwGetTime();
    }
}

#endif //FLK_TIME_HPP
