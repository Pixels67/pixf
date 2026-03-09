#ifndef FLK_VECTOR_HPP
#define FLK_VECTOR_HPP

#include <cmath>

#include "Common.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"

namespace Flock {
    class Quaternion;

    template<typename T>
    struct FLK_API Vector2 {
        T x, y;

        Vector2() : x(0), y(0) {
        }

        Vector2(T x, T y) : x(x), y(y) {
        }

        explicit Vector2(T s) : x(s), y(s) {
        }

        template<typename U>
        Vector2(const Vector2<U> &other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {
        }

        static Vector2 Zero() { return Vector2(0, 0); }
        static Vector2 One() { return Vector2(1, 1); }
        static Vector2 Up() { return Vector2(0, 1); }
        static Vector2 Down() { return Vector2(0, -1); }
        static Vector2 Right() { return Vector2(1, 0); }
        static Vector2 Left() { return Vector2(-1, 0); }

        Vector2 operator-() const {
            return {-x, -y};
        }

#define EXPR(op) (x op o.x), (y op o.y)
        FLK_MATH_BINARY_OPS(Vector2, Vector2, EXPR)
#undef EXPR
#define EXPR(op)                                                                                                       \
    x op o.x;                                                                                                          \
    y op o.y
        FLK_MATH_ASSIGN_OPS(Vector2, Vector2, EXPR)
#undef EXPR
#define EXPR(op) (x op o), (y op o)
        FLK_MATH_BINARY_OPS(Vector2, T, EXPR)
#undef EXPR
#define EXPR(op)                                                                                                       \
    x op o;                                                                                                            \
    y op o
        FLK_MATH_ASSIGN_OPS(Vector2, T, EXPR)
#undef EXPR

        T Dot(const Vector2 &other) const {
            return x * other.x + y * other.y;
        }

        f64     SqrMagnitude() const { return x * x + y * y; }
        f64     Magnitude() const { return sqrt(SqrMagnitude()); }
        Vector2 Normalized() const { return *this / Magnitude(); }

        Vector2 &Normalize() {
            auto mag = Magnitude();
            x        /= mag;
            y        /= mag;
            return *this;
        }

        bool operator==(const Vector2 &vector2) const = default;
    };

    template<typename T>
    struct Vector3 {
        T x, y, z;

        Vector3() : x(0), y(0), z(0) {
        }

        Vector3(T x, T y, T z) : x(x), y(y), z(z) {
        }

        explicit Vector3(T s) : x(s), y(s), z(s) {
        }

        template<typename U>
        Vector3(const Vector3<U> &other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)),
                                           z(static_cast<T>(other.z)) {
        }

        static Vector3 Zero() { return Vector3(0, 0, 0); }
        static Vector3 One() { return Vector3(1, 1, 1); }
        static Vector3 Up() { return Vector3(0, 1, 0); }
        static Vector3 Down() { return Vector3(0, -1, 0); }
        static Vector3 Right() { return Vector3(1, 0, 0); }
        static Vector3 Left() { return Vector3(-1, 0, 0); }
        static Vector3 Forward() { return Vector3(0, 0, 1); }
        static Vector3 Back() { return Vector3(0, 0, -1); }

        Vector3 operator-() const {
            return {-x, -y, -z};
        }

#define EXPR(op) (x op o.x), (y op o.y), (z op o.z)
        FLK_MATH_BINARY_OPS(Vector3, Vector3, EXPR)
#undef EXPR
#define EXPR(op)                                                                                                       \
    x op o.x;                                                                                                          \
    y op o.y;                                                                                                          \
    z op o.z
        FLK_MATH_ASSIGN_OPS(Vector3, Vector3, EXPR)
#undef EXPR
#define EXPR(op) (x op o), (y op o), (z op o)
        FLK_MATH_BINARY_OPS(Vector3, T, EXPR)
#undef EXPR
#define EXPR(op)                                                                                                       \
    x op o;                                                                                                            \
    y op o;                                                                                                            \
    z op o
        FLK_MATH_ASSIGN_OPS(Vector3, T, EXPR)
#undef EXPR

        Vector3 operator*(const Quaternion &quaternion) { return quaternion * *this; }

        T Dot(const Vector3 &other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        Vector3 Cross(const Vector3 &other) const {
            return {
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            };
        }

        f64     SqrMagnitude() const { return x * x + y * y + z * z; }
        f64     Magnitude() const { return sqrt(SqrMagnitude()); }
        Vector3 Normalized() const { return *this / Magnitude(); }

        Vector3 &Normalize() {
            auto mag = Magnitude();
            x        /= mag;
            y        /= mag;
            z        /= mag;
            return *this;
        }
    };

    template<typename T>
    struct Vector4 {
        T x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {
        }

        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {
        }

        explicit Vector4(T s) : x(s), y(s), z(s), w(s) {
        }

        template<typename U>
        Vector4(const Vector4<U> &other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)),
                                           z(static_cast<T>(other.z)),
                                           w(static_cast<T>(other.w)) {
        }

        static Vector4 Zero() { return Vector4(0, 0, 0, 0); }
        static Vector4 One() { return Vector4(1, 1, 1, 1); }

        Vector4 operator-() const {
            return {-x, -y, -z, -w};
        }

#define EXPR(op) (x op o.x), (y op o.y), (z op o.z), (w op o.w)
        FLK_MATH_BINARY_OPS(Vector4, Vector4, EXPR)
#undef EXPR
#define EXPR(op)                                                                                                       \
    x op o.x;                                                                                                          \
    y op o.y;                                                                                                          \
    z op o.z;                                                                                                          \
    w op o.w
        FLK_MATH_ASSIGN_OPS(Vector4, Vector4, EXPR)
#undef EXPR
#define EXPR(op) (x op o), (y op o), (z op o), (w op o)
        FLK_MATH_BINARY_OPS(Vector4, T, EXPR)
#undef EXPR
#define EXPR(op)                                                                                                       \
    x op o;                                                                                                            \
    y op o;                                                                                                            \
    z op o;                                                                                                            \
    w op o
        FLK_MATH_ASSIGN_OPS(Vector4, T, EXPR)
#undef EXPR

        T Dot(const Vector4 &other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        f64     SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
        f64     Magnitude() const { return sqrt(SqrMagnitude()); }
        Vector4 Normalized() const { return *this / Magnitude(); }

        Vector4 &Normalize() {
            auto mag = Magnitude();
            x        /= mag;
            y        /= mag;
            z        /= mag;
            w        /= mag;
            return *this;
        }
    };

    template<typename T>
    auto Reflect(Vector2<T> &vector) {
        return Reflectable{
            "Vector2",
            std::make_tuple(
                Field("x", &vector.x),
                Field("y", &vector.y)
            )
        };
    }

    template<typename T>
    auto Reflect(Vector3<T> &vector) {
        return Reflectable{
            "Vector3",
            std::make_tuple(
                Field("x", &vector.x),
                Field("y", &vector.y),
                Field("z", &vector.z)
            )
        };
    }

    template<typename T>
    auto Reflect(Vector4<T> &vector) {
        return Reflectable{
            "Vector4",
            std::make_tuple(
                Field("x", &vector.x),
                Field("y", &vector.y),
                Field("z", &vector.z),
                Field("w", &vector.w)
            )
        };
    }

    using Vector2i = Vector2<i32>;
    using Vector3i = Vector3<i32>;
    using Vector4i = Vector4<i32>;

    using Vector2u = Vector2<u32>;
    using Vector3u = Vector3<u32>;
    using Vector4u = Vector4<u32>;

    using Vector2f = Vector2<f32>;
    using Vector3f = Vector3<f32>;
    using Vector4f = Vector4<f32>;

    using Vector2d = Vector2<f64>;
    using Vector3d = Vector3<f64>;
    using Vector4d = Vector4<f64>;
} // namespace Flock
#endif // FLK_VECTOR_HPP
