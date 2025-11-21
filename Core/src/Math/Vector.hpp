#ifndef PIXF_VECTOR_HPP
#define PIXF_VECTOR_HPP

#include <cmath>

#include "Math.hpp"

namespace Pixf::Core::Math {
    template<typename T>
    struct Vector2 {
        T x, y;

        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}
        explicit Vector2(T s) : x(s), y(s) {}

        template<typename U>
        Vector2(const Vector2<U> &other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

        static Vector2 Zero() { return Vector2(0, 0); }
        static Vector2 One() { return Vector2(1, 1); }
        static Vector2 Up() { return Vector2(0, 1); }
        static Vector2 Down() { return Vector2(0, -1); }
        static Vector2 Right() { return Vector2(1, 0); }
        static Vector2 Left() { return Vector2(-1, 0); }

#define EXPR(op) (x op o.x), (y op o.y)
        PIXF_MATH_BINARY_OPS(Vector2, Vector2, EXPR)
#define EXPR(op)                                                                                                       \
    x op o.x;                                                                                                          \
    y op o.y
        PIXF_MATH_ASSIGN_OPS(Vector2, Vector2, EXPR)
#define EXPR(op) (x op o), (y op o)
        PIXF_MATH_BINARY_OPS(Vector2, T, EXPR)
#define EXPR(op)                                                                                                       \
    x op o;                                                                                                            \
    y op o
        PIXF_MATH_ASSIGN_OPS(Vector2, T, EXPR)
#undef EXPR

        double SqrMagnitude() const { return x * x + y * y; }
        double Magnitude() const { return sqrt(SqrMagnitude()); }
        Vector2 Normalized() const { return *this / Magnitude(); }

        Vector2 &Normalize() {
            auto mag = Magnitude();
            x /= mag;
            y /= mag;
            return *this;
        }
    };

    template<typename T>
    struct Vector3 {
        T x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
        explicit Vector3(T s) : x(s), y(s), z(s) {}

        template<typename U>
        Vector3(const Vector3<U> &other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)) {}

        static Vector3 Zero() { return Vector3(0, 0, 0); }
        static Vector3 One() { return Vector3(1, 1, 1); }
        static Vector3 Up() { return Vector3(0, 1, 0); }
        static Vector3 Down() { return Vector3(0, -1, 0); }
        static Vector3 Right() { return Vector3(1, 0, 0); }
        static Vector3 Left() { return Vector3(-1, 0, 0); }
        static Vector3 Forward() { return Vector3(0, 0, 1); }
        static Vector3 Back() { return Vector3(0, 0, -1); }

#define EXPR(op) (x op o.x), (y op o.y), (z op o.z)
        PIXF_MATH_BINARY_OPS(Vector3, Vector3, EXPR)
#define EXPR(op)                                                                                                       \
    x op o.x;                                                                                                          \
    y op o.y;                                                                                                          \
    z op o.z
        PIXF_MATH_ASSIGN_OPS(Vector3, Vector3, EXPR)
#define EXPR(op) (x op o), (y op o), (z op o)
        PIXF_MATH_BINARY_OPS(Vector3, T, EXPR)
#define EXPR(op)                                                                                                       \
    x op o;                                                                                                            \
    y op o;                                                                                                            \
    z op o
        PIXF_MATH_ASSIGN_OPS(Vector3, T, EXPR)
#undef EXPR

        double SqrMagnitude() const { return x * x + y * y + z * z; }
        double Magnitude() const { return sqrt(SqrMagnitude()); }
        Vector3 Normalized() const { return *this / Magnitude(); }

        Vector3 &Normalize() {
            auto mag = Magnitude();
            x /= mag;
            y /= mag;
            z /= mag;
            return *this;
        }
    };

    template<typename T>
    struct Vector4 {
        T x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        explicit Vector4(T s) : x(s), y(s), z(s), w(s) {}

        template<typename U>
        Vector4(const Vector4<U> &other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)),
            w(static_cast<T>(other.w)) {}

        static Vector4 Zero() { return Vector4(0, 0, 0, 0); }
        static Vector4 One() { return Vector4(1, 1, 1, 1); }

#define EXPR(op) (x op o.x), (y op o.y), (z op o.z), (w op o.w)
        PIXF_MATH_BINARY_OPS(Vector4, Vector4, EXPR)
#define EXPR(op)                                                                                                       \
    x op o.x;                                                                                                          \
    y op o.y;                                                                                                          \
    z op o.z;                                                                                                          \
    w op o.w
        PIXF_MATH_ASSIGN_OPS(Vector4, Vector4, EXPR)
#define EXPR(op) (x op o), (y op o), (z op o), (w op o)
        PIXF_MATH_BINARY_OPS(Vector4, T, EXPR)
#define EXPR(op)                                                                                                       \
    x op o;                                                                                                            \
    y op o;                                                                                                            \
    z op o;                                                                                                            \
    w op o
        PIXF_MATH_ASSIGN_OPS(Vector4, T, EXPR)
#undef EXPR

        double SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
        double Magnitude() const { return sqrt(SqrMagnitude()); }
        Vector4 Normalized() const { return *this / Magnitude(); }

        Vector4 &Normalize() {
            auto mag = Magnitude();
            x /= mag;
            y /= mag;
            z /= mag;
            w /= mag;
            return *this;
        }
    };

    using Vector2i = Vector2<int>;
    using Vector3i = Vector3<int>;
    using Vector4i = Vector4<int>;

    using Vector2u = Vector2<unsigned>;
    using Vector3u = Vector3<unsigned>;
    using Vector4u = Vector4<unsigned>;

    using Vector2f = Vector2<float>;
    using Vector3f = Vector3<float>;
    using Vector4f = Vector4<float>;

    using Vector2d = Vector2<double>;
    using Vector3d = Vector3<double>;
    using Vector4d = Vector4<double>;
} // namespace Pixf::Core::Math
#endif // PIXF_VECTOR_HPP
