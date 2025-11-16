#ifndef PIXF_MATH_HPP
#define PIXF_MATH_HPP

#include "Common.hpp"

namespace Pixf::Core::Math {
#define PIXF_MATH_BINARY_OPS(T1, T2, ...)                                                                              \
    T1 operator+(const T2 &o) const { return T1(__VA_ARGS__(+)); }                                                     \
    T1 operator-(const T2 &o) const { return T1(__VA_ARGS__(-)); }                                                     \
    T1 operator*(const T2 &o) const { return T1(__VA_ARGS__(*)); }                                                     \
    T1 operator/(const T2 &o) const { return T1(__VA_ARGS__(/)); }

#define PIXF_MATH_ASSIGN_OPS(T1, T2, ...)                                                                              \
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
} // namespace Pixf::Core::Math

PIXF_API double RadiansToDegrees(double angle);
PIXF_API double DegreesToRadians(double angle);

#endif // PIXF_MATH_HPP
