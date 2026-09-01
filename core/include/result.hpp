#ifndef FLOCK_RESULT_HPP
#define FLOCK_RESULT_HPP

#include <algorithm>

#include "common.hpp"

namespace flock {
    template <typename T, typename E>
    class result {
        union {
            T value_;
            E error_ = {};
        };

        bool success_ = false;

    public:
        result(const T &value) : value_(value), success_(true) {
        }

        result(T &&value) : value_(std::move(value)), success_(true) {
        }

        result(E error) : error_(error) {
        }

        ~result() {
            if (success_) {
                value_.~T();
            } else {
                error_.~E();
            }
        }

        bool success() const {
            return success_;
        }

        operator bool() const {
            return success();
        }

        T &get() {
            if (!success()) {
                FLK_ASSERT(false, "Called get on unsuccessful result");
            }

            return value_;
        }

        E get_err() {
            if (success()) {
                FLK_ASSERT(false, "Called get_err on successful result");
            }

            return error_;
        }
    };
}

#endif //FLOCK_RESULT_HPP
