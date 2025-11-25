#ifndef PIXF_COLOR_HPP
#define PIXF_COLOR_HPP

#include "Math.hpp"
#include "Vector.hpp"

namespace Pixf::Core::Math {
    template<typename T>
    struct Color3 {
        T r, g, b;

        Color3() : r(0), g(0), b(0) {}
        Color3(T r, T g, T b) : r(r), g(g), b(b) {}
        explicit Color3(T s) : r(s), g(s), b(s) {}

        template<typename U>
        Color3(const Color3<U> &other) :
            r(static_cast<T>(other.r)), g(static_cast<T>(other.g)), b(static_cast<T>(other.b)) {}

        static Color3 Black() { return Color3(0, 0, 0); }
        static Color3 White() { return Color3(255, 255, 255); }

        static Color3 Red() { return Color3(255, 0, 0); }
        static Color3 Blue() { return Color3(0, 255, 0); }
        static Color3 Green() { return Color3(0, 0, 255); }

        static Color3 Yellow() { return Color3(255, 255, 0); }
        static Color3 Cyan() { return Color3(0, 255, 255); }
        static Color3 Magenta() { return Color3(255, 0, 255); }

        Vector3f ToVector() {
            return Vector3f(static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255);
        }

        void FromVector(const Vector3f &v) {
            r = static_cast<T>(v.x * 255);
            g = static_cast<T>(v.y * 255);
            b = static_cast<T>(v.z * 255);
        }

#define EXPR(op) (r op o.r), (g op o.g), (b op o.b)
        PIXF_MATH_BINARY_OPS(Color3, Color3, EXPR)
#define EXPR(op)                                                                                                       \
    r op o.r;                                                                                                          \
    g op o.g;                                                                                                          \
    b op o.b
        PIXF_MATH_ASSIGN_OPS(Color3, Color3, EXPR)
#define EXPR(op) (r op o), (g op o), (b op o)
        PIXF_MATH_BINARY_OPS(Color3, T, EXPR)
#define EXPR(op)                                                                                                       \
    r op o;                                                                                                            \
    g op o;                                                                                                            \
    b op o
        PIXF_MATH_ASSIGN_OPS(Color3, T, EXPR)
#undef EXPR

        template<typename Archive>
        static void Serialize(Archive &archive, Color3 &color) {
            archive("r", color.r);
            archive("g", color.g);
            archive("b", color.b);
        }
    };

    template<typename T>
    struct Color4 {
        T r, g, b, a;

        Color4() : r(0), g(0), b(0), a(0) {}
        Color4(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}
        explicit Color4(T s) : r(s), g(s), b(s), a(s) {}

        template<typename U>
        Color4(const Color4<U> &other) :
            r(static_cast<T>(other.r)), g(static_cast<T>(other.g)), b(static_cast<T>(other.b)),
            a(static_cast<T>(other.a)) {}

        static Color4 Black() { return Color4(0, 0, 0, 255); }
        static Color4 White() { return Color4(255, 255, 255, 255); }

        static Color4 Red() { return Color4(255, 0, 0, 255); }
        static Color4 Blue() { return Color4(0, 255, 0, 255); }
        static Color4 Green() { return Color4(0, 0, 255, 255); }

        static Color4 Yellow() { return Color4(255, 255, 0, 255); }
        static Color4 Cyan() { return Color4(0, 255, 255, 255); }
        static Color4 Magenta() { return Color4(255, 0, 255, 255); }

        static Color4 Transparent() { return Color4(0, 0, 0, 0); }

        Vector4f ToVector() {
            return Vector4f(static_cast<float>(r) / 255,
                            static_cast<float>(g) / 255,
                            static_cast<float>(b) / 255,
                            static_cast<float>(a) / 255);
        }

        void FromVector(const Vector4f &v) {
            r = static_cast<T>(v.x * 255);
            g = static_cast<T>(v.y * 255);
            b = static_cast<T>(v.z * 255);
            a = static_cast<T>(v.w * 255);
        }

#define EXPR(op) (r op o.r), (g op o.g), (b op o.b), (a op o.a)
        PIXF_MATH_BINARY_OPS(Color4, Color4, EXPR)
#define EXPR(op)                                                                                                       \
    r op o.r;                                                                                                          \
    g op o.g;                                                                                                          \
    b op o.b;                                                                                                          \
    a op o.a
        PIXF_MATH_ASSIGN_OPS(Color4, Color4, EXPR)
#define EXPR(op) (r op o), (g op o), (b op o), (a op o)
        PIXF_MATH_BINARY_OPS(Color4, T, EXPR)
#define EXPR(op)                                                                                                       \
    r op o;                                                                                                            \
    g op o;                                                                                                            \
    b op o;                                                                                                            \
    a op o
        PIXF_MATH_ASSIGN_OPS(Color4, T, EXPR)
#undef EXPR
    };

    using Color3u8 = Color3<uint8_t>;
    using Color4u8 = Color4<uint8_t>;

    PIXF_SERIALIZE(Color3u8,
        PIXF_FIELD(r, Color3u8.r);
        PIXF_FIELD(g, Color3u8.g);
        PIXF_FIELD(b, Color3u8.b);
    )

    PIXF_SERIALIZE(Color4u8,
        PIXF_FIELD(r, Color4u8.r);
        PIXF_FIELD(g, Color4u8.g);
        PIXF_FIELD(b, Color4u8.b);
        PIXF_FIELD(a, Color4u8.a);
    )
} // namespace Pixf::Core::Math

#endif // PIXF_COLOR_HPP
