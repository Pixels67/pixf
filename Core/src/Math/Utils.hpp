#ifndef FLK_UTILS_HPP
#define FLK_UTILS_HPP

#include "Common.hpp"

namespace Flock {
#define FLK_MATH_BINARY_OPS(T1, T2, ...)                                                                               \
    T1 operator+(const T2 &o) const { return T1(__VA_ARGS__(+)); }                                                     \
    T1 operator-(const T2 &o) const { return T1(__VA_ARGS__(-)); }                                                     \
    T1 operator*(const T2 &o) const { return T1(__VA_ARGS__(*)); }                                                     \
    T1 operator/(const T2 &o) const { return T1(__VA_ARGS__(/)); }

#define FLK_MATH_ASSIGN_OPS(T1, T2, ...)                                                                               \
    T1 &operator+=(const T2 &o) {                                                                                      \
        __VA_ARGS__(+=);                                                                                               \
        return *this;                                                                                                  \
    }                                                                                                                  \
    T1 &operator-=(const T2 &o) {                                                                                      \
        __VA_ARGS__(-=);                                                                                               \
        return *this;                                                                                                  \
    }                                                                                                                  \
    T1 &operator*=(const T2 &o) {                                                                                      \
        __VA_ARGS__(*=);                                                                                               \
        return *this;                                                                                                  \
    }                                                                                                                  \
    T1 &operator/=(const T2 &o) {                                                                                      \
        __VA_ARGS__(/=);                                                                                               \
        return *this;                                                                                                  \
    }

    f64 RadiansToDegrees(f64 angleRadians);
    f64 DegreesToRadians(f64 angleDegrees);
    f64 GetPi();
} // namespace Flock

#endif // FLK_UTILS_HPP
