#ifndef FLOCK_MAYBE_HPP
#define FLOCK_MAYBE_HPP

#include "common.hpp"

namespace flock {
    template <typename T>
    class maybe {
        T    value_     = {};
        bool has_value_ = false;

    public:
        maybe() = default;

        maybe(T value) : value_(value), has_value_(true) {
        }

        bool has_value() const {
            return has_value_;
        }

        operator bool() const {
            return has_value();
        }

        T get() {
            if (!has_value()) {
                FLK_ASSERT(false, "Called get on invalid maybe");
            }

            return value_;
        }
    };
}

#endif //FLOCK_MAYBE_HPP
