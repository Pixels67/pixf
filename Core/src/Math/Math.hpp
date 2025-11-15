#ifndef MATH_HPP
#define MATH_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Serialization/Serializable.hpp"

namespace Pixf::Core::Math {

#define PIXF_MATH_GLM_OVERLOADS(type, rhs)                                                                             \
    type operator+(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() + other.ToGlm();                                                                      \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type operator-(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() - other.ToGlm();                                                                      \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type operator*(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() * other.ToGlm();                                                                      \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type operator/(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() / other.ToGlm();                                                                      \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type &operator+=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() + other.ToGlm());                                                                        \
        return *this;                                                                                                  \
    }                                                                                                                  \
    type &operator-=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() - other.ToGlm());                                                                        \
        return *this;                                                                                                  \
    }                                                                                                                  \
    type &operator*=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() * other.ToGlm());                                                                        \
        return *this;                                                                                                  \
    }                                                                                                                  \
    type &operator/=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() / other.ToGlm());                                                                        \
        return *this;                                                                                                  \
    }

#define PIXF_MATH_OVERLOADS(type, rhs)                                                                                 \
    type operator+(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() + other;                                                                              \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type operator-(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() - other;                                                                              \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type operator*(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() * other;                                                                              \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type operator/(const rhs &other) const {                                                                           \
        auto vec = this->ToGlm() / other;                                                                              \
        return type(vec);                                                                                              \
    }                                                                                                                  \
    type &operator+=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() + other);                                                                                \
        return *this;                                                                                                  \
    }                                                                                                                  \
    type &operator-=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() - other);                                                                                \
        return *this;                                                                                                  \
    }                                                                                                                  \
    type &operator*=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() * other);                                                                                \
        return *this;                                                                                                  \
    }                                                                                                                  \
    type &operator/=(const rhs &other) {                                                                               \
        FromGlm(this->ToGlm() / other);                                                                                \
        return *this;                                                                                                  \
    }

    template<typename T>
    std::string GetTypePostfix() {
        return "";
    }

    template<>
    inline std::string GetTypePostfix<float>() {
        return "f";
    }

    template<>
    inline std::string GetTypePostfix<double>() {
        return "d";
    }

    template<>
    inline std::string GetTypePostfix<int>() {
        return "i";
    }

    template<>
    inline std::string GetTypePostfix<unsigned>() {
        return "u";
    }

    template<>
    inline std::string GetTypePostfix<uint8_t>() {
        return "u8";
    }

    template<>
    inline std::string GetTypePostfix<uint16_t>() {
        return "u16";
    }

    template<typename T = float>
    struct Vector2 {
        T x, y;

        Vector2() : x(0), y(0) {}
        explicit Vector2(const T s) : x(s), y(s) {}
        explicit Vector2(glm::vec2 vec) : x(vec.x), y(vec.y) {}
        Vector2(const T x, const T y) : x(x), y(y) {}

        template<typename U>
        Vector2(const Vector2<U> &other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

        PIXF_MATH_GLM_OVERLOADS(Vector2, Vector2)
        PIXF_MATH_OVERLOADS(Vector2, T)

        static Vector2 Up() { return Vector2(0, 1); }
        static Vector2 Down() { return Vector2(0, -1); }
        static Vector2 Right() { return Vector2(1, 0); }
        static Vector2 Left() { return Vector2(-1, 0); }

        static float Distance(const Vector2 &lhs, const Vector2 &rhs) {
            return glm::distance(lhs.ToGlm(), rhs.ToGlm());
        }

        T Magnitude() const { return glm::length(ToGlm()); }
        T MagnitudeSquared() const { return glm::length(ToGlm()) * glm::length(ToGlm()); }

        Serialization::Json::object Serialize(bool editorMode) { return Serialize(); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode) {
            Deserialize(json);
        }

        Serialization::Json::object Serialize() {
            Serialization::Json::object json;

            const std::string postfix = GetTypePostfix<T>();

            json["Vector2" + postfix] = Serialization::Json::object();
            json["Vector2" + postfix].as_object()["x"] = x;
            json["Vector2" + postfix].as_object()["y"] = x;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json) {
            const std::string postfix = GetTypePostfix<T>();

            if (!json.contains("Vector2" + postfix)) {
                return;
            }

            x = json.at("Vector2" + postfix).as_object().at("x").to_number<T>();
            y = json.at("Vector2" + postfix).as_object().at("y").to_number<T>();
        }

        glm::vec<2, T> ToGlm() const { return glm::vec<2, T>(x, y); }
        void FromGlm(glm::vec<2, T> glm) {
            x = glm.x;
            y = glm.y;
        }
    };

    template<typename T = float>
    struct Vector3 {
        T x = 0, y = 0, z = 0;

        Vector3() {}
        explicit Vector3(const T s) : x(s), y(s), z(s) {}
        explicit Vector3(const glm::vec<3, T> vec) : x(vec.x), y(vec.y), z(vec.z) {}
        Vector3(const T x, const T y, const T z) : x(x), y(y), z(z) {}

        template<typename U>
        Vector3(const Vector3<U> &other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)) {}

        static Vector3 Up() { return Vector3(0, 1, 0); }
        static Vector3 Down() { return Vector3(0, -1, 0); }
        static Vector3 Right() { return Vector3(1, 0, 0); }
        static Vector3 Left() { return Vector3(-1, 0, 0); }
        static Vector3 Forward() { return Vector3(0, 0, 1); }
        static Vector3 Back() { return Vector3(0, 0, -1); }

        static float Distance(const Vector3 &lhs, const Vector3 &rhs) {
            return glm::distance(lhs.ToGlm(), rhs.ToGlm());
        }

        PIXF_MATH_GLM_OVERLOADS(Vector3, Vector3)
        PIXF_MATH_OVERLOADS(Vector3, T)

        Vector3 &Normalize() {
            FromGlm(glm::normalize(ToGlm()));
            return *this;
        }

        T Magnitude() const { return glm::length(ToGlm()); }
        T MagnitudeSquared() const { return glm::length(ToGlm()) * glm::length(ToGlm()); }

        Serialization::Json::object Serialize(bool editorMode) { return Serialize(); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode) {
            Deserialize(json);
        }

        Serialization::Json::object Serialize() {
            Serialization::Json::object json;

            const std::string postfix = GetTypePostfix<T>();

            json["Vector3" + postfix] = Serialization::Json::object();
            json["Vector3" + postfix].as_object()["x"] = x;
            json["Vector3" + postfix].as_object()["y"] = y;
            json["Vector3" + postfix].as_object()["z"] = z;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json) {
            const std::string postfix = GetTypePostfix<T>();

            if (!json.contains("Vector3" + postfix)) {
                return;
            }

            x = json.at("Vector3" + postfix).as_object().at("x").to_number<T>();
            y = json.at("Vector3" + postfix).as_object().at("y").to_number<T>();
            z = json.at("Vector3" + postfix).as_object().at("z").to_number<T>();
        }

        glm::vec<3, T> ToGlm() const { return glm::vec<3, T>(x, y, z); }
        void FromGlm(glm::vec<3, T> glm) {
            x = glm.x;
            y = glm.y;
            z = glm.z;
        }
    };

    template<typename T = float>
    struct Vector4 {
        T x = 0, y = 0, z = 0, w = 0;

        Vector4() {}
        explicit Vector4(const T s) : x(s), y(s), z(s), w(s) {}
        explicit Vector4(const glm::vec<4, T> vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
        Vector4(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) {}

        template<typename U>
        Vector4(const Vector4<U> &other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)),
            w(static_cast<T>(other.w)) {}

        static float Distance(const Vector4 &lhs, const Vector4 &rhs) {
            return glm::distance(lhs.ToGlm(), rhs.ToGlm());
        }

        PIXF_MATH_GLM_OVERLOADS(Vector4, Vector4)
        PIXF_MATH_OVERLOADS(Vector4, T)

        Vector4 &Normalize() {
            glm::vec<4, T> vec = glm::normalize(ToGlm());
            FromGlm(vec);
            return *this;
        }

        T Magnitude() const { return glm::length(ToGlm()); }
        T MagnitudeSquared() const { return glm::length(ToGlm()) * glm::length(ToGlm()); }

        Serialization::Json::object Serialize(bool editorMode) { return Serialize(); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode) {
            Deserialize(json);
        }

        Serialization::Json::object Serialize() {
            Serialization::Json::object json;

            const std::string postfix = GetTypePostfix<T>();

            json["Vector4" + postfix] = Serialization::Json::object();
            json["Vector4" + postfix].as_object()["x"] = x;
            json["Vector4" + postfix].as_object()["y"] = x;
            json["Vector4" + postfix].as_object()["z"] = z;
            json["Vector4" + postfix].as_object()["w"] = w;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json) {
            const std::string postfix = GetTypePostfix<T>();

            if (!json.contains("Vector4" + postfix)) {
                return;
            }

            x = json.at("Vector4" + postfix).as_object().at("x").to_number<T>();
            y = json.at("Vector4" + postfix).as_object().at("y").to_number<T>();
            z = json.at("Vector4" + postfix).as_object().at("z").to_number<T>();
            w = json.at("Vector4" + postfix).as_object().at("w").to_number<T>();
        }

        glm::vec<4, T> ToGlm() const { return glm::vec<4, T>(x, y, z, w); }
        void FromGlm(glm::vec<4, T> glm) {
            x = glm.x;
            y = glm.y;
            z = glm.z;
            w = glm.w;
        }
    };

    template<typename T = uint8_t>
    struct Color3 {
        T r = 0, g = 0, b = 0;

        Color3() {}
        explicit Color3(const T s) : r(s), g(s), b(s) {}
        explicit Color3(const glm::vec3 &vec) : r(vec.x), g(vec.y), b(vec.z) {}
        Color3(const T r, const T g, const T b) : r(r), g(g), b(b) {}

        template<typename U>
        Color3(const Color3<U> &other) :
            r(static_cast<T>(other.r)), g(static_cast<T>(other.g)), b(static_cast<T>(other.b)) {}

        template<typename U>
        Color3(const Vector3<U> &other) :
            r(static_cast<T>(other.x)), g(static_cast<T>(other.y)), b(static_cast<T>(other.z)) {}

        PIXF_MATH_GLM_OVERLOADS(Color3, Color3)
        PIXF_MATH_OVERLOADS(Color3, T)

        Serialization::Json::object Serialize(bool editorMode) { return Serialize(); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode) {
            Deserialize(json);
        }

        Serialization::Json::object Serialize() {
            Serialization::Json::object json;

            const std::string postfix = GetTypePostfix<T>();

            json["Color3" + postfix] = Serialization::Json::object();
            json["Color3" + postfix].as_object()["r"] = r;
            json["Color3" + postfix].as_object()["g"] = g;
            json["Color3" + postfix].as_object()["b"] = b;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json) {
            const std::string postfix = GetTypePostfix<T>();

            if (!json.contains("Color3" + postfix)) {
                return;
            }

            r = json.at("Color3" + postfix).as_object().at("r").to_number<T>();
            g = json.at("Color3" + postfix).as_object().at("g").to_number<T>();
            b = json.at("Color3" + postfix).as_object().at("b").to_number<T>();
        }

        glm::vec<3, T> ToGlm() const { return glm::vec<3, T>(r, g, b); }
        void FromGlm(glm::vec<3, T> glm) {
            r = glm.x;
            g = glm.y;
            b = glm.z;
        }
    };

    template<typename T = uint8_t>
    struct Color4 {
        T r = 0, g = 0, b = 0, a = 0;

        Color4() {}
        explicit Color4(const T s) : r(s), g(s), b(s), a(s) {}
        explicit Color4(const glm::vec<4, T> vec) : r(vec.x), g(vec.y), b(vec.z), a(vec.w) {}
        Color4(const T r, const T g, const T b, const T a) : r(r), g(g), b(b), a(a) {}

        template<typename U>
        Color4(const Color4<U> &other) :
            r(static_cast<T>(other.r)), g(static_cast<T>(other.g)), b(static_cast<T>(other.b)),
            a(static_cast<T>(other.a)) {}

        template<typename U>
        Color4(const Vector4<U> &other) :
            r(static_cast<T>(other.x)), g(static_cast<T>(other.y)), b(static_cast<T>(other.z)),
            a(static_cast<T>(other.w)) {}

        PIXF_MATH_GLM_OVERLOADS(Color4, Color4)
        PIXF_MATH_OVERLOADS(Color4, T)

        Serialization::Json::object Serialize(bool editorMode) { return Serialize(); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode) {
            return Deserialize(json);
        }

        Serialization::Json::object Serialize() {
            Serialization::Json::object json;

            const std::string postfix = GetTypePostfix<T>();

            json["Color4" + postfix] = Serialization::Json::object();
            json["Color4" + postfix].as_object()["r"] = r;
            json["Color4" + postfix].as_object()["g"] = g;
            json["Color4" + postfix].as_object()["b"] = b;
            json["Color4" + postfix].as_object()["a"] = a;

            return json;
        }

        void Deserialize(const Serialization::Json::object &json) {
            const std::string postfix = GetTypePostfix<T>();

            if (!json.contains("Color4" + postfix)) {
                return;
            }

            r = json.at("Color4" + postfix).as_object().at("r").to_number<T>();
            g = json.at("Color4" + postfix).as_object().at("g").to_number<T>();
            b = json.at("Color4" + postfix).as_object().at("b").to_number<T>();
            a = json.at("Color4" + postfix).as_object().at("a").to_number<T>();
        }

        glm::vec<4, T> ToGlm() const { return glm::vec<4, T>(r, g, b, a); }
        void FromGlm(glm::vec<4, T> glm) {
            r = glm.x;
            g = glm.y;
            b = glm.z;
            a = glm.z;
        }
    };

    template<typename T = float>
    struct Quat {
        T x = 0, y = 0, z = 0, w = 1;

        Quat() {}
        explicit Quat(T s) : x(0), y(0), z(0), w(s) {}
        explicit Quat(glm::qua<T> quat) : x(quat.x), y(quat.y), z(quat.z), w(quat.w) {}
        Quat(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) {}

        template<typename U>
        Quat(const Quat<U> &other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)),
            w(static_cast<T>(other.w)) {}

        static Quat Identity() { return Quat(); }
        static Quat Zero() { return Quat(0, 0, 0, 0); }
        static Quat AngleAxis(T angle, Vector3<T> axis) {
            return Quat(glm::angleAxis(angle, axis.Normalize().ToGlm()));
        }
        static Quat Euler(Vector3<T> eulerAngles) { return Quat(glm::qua<T>(glm::radians(eulerAngles.ToGlm()))); }
        static Quat LookAt(Vector3<T> direction, Vector3<T> up) {
            return Quat(glm::quatLookAt(direction.Normalize().ToGlm(), up.Normalize().ToGlm()));
        }

        PIXF_MATH_GLM_OVERLOADS(Quat, Quat)
        PIXF_MATH_OVERLOADS(Quat, T)

        Vector3<T> operator*(Vector3<T> vector) const {
            glm::vec<3, T> vec = ToGlm() * vector.ToGlm();
            return Vector3<T>(vec.x, vec.y, vec.z);
        }

        Quat &Normalize() {
            glm::qua<T> quat = glm::normalize(ToGlm());
            FromGlm(quat);
            return *this;
        }

        Quat &Inverse() {
            glm::qua<T> quat = glm::inverse(ToGlm());
            FromGlm(quat);
            return *this;
        }

        Quat &Rotate(const Quat quaternion) {
            glm::qua<T> quat = glm::normalize(quaternion.ToGlm() * ToGlm());
            FromGlm(quat);
            return *this;
        }

        Quat &RotateAround(T angle, Vector3<T> axis) {
            glm::qua<T> quat = glm::normalize(ToGlm() * glm::angleAxis(glm::radians(angle), axis.Normalize().ToGlm()));
            FromGlm(quat);
            return *this;
        }

        Vector3<T> EulerAngles() const {
            auto euler = glm::degrees(glm::eulerAngles(ToGlm()));
            return Vector3<T>(euler.x, euler.y, euler.z);
        }

        Serialization::Json::object Serialize(bool editorMode) { return Serialize(); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager, bool editorMode) {
            Deserialize(json);
        }

        Serialization::Json::object Serialize() {
            Vector3<T> euler = EulerAngles();
            return euler.Serialize();
        }

        void Deserialize(const Serialization::Json::object &json) {
            Vector3<T> euler;
            euler.Deserialize(json);

            *this = Euler(euler);
        }

        glm::qua<T> ToGlm() const { return glm::qua<T>(w, x, y, z); }
        void FromGlm(glm::qua<T> glm) {
            x = glm.x;
            y = glm.y;
            z = glm.z;
            w = glm.w;
        }
    };

    template<typename T = float>
    struct Matrix4 {
        T elements[4][4];

        Matrix4() { FromGlm(glm::mat<4, 4, T>(1.0F)); }
        Matrix4(glm::mat<4, 4, T> mat) { FromGlm(mat); }

        template<typename U>
        Matrix4(const Matrix4<U> &other) {
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    elements[row][col] = static_cast<T>(other.elements[row][col]);
                }
            }
        }

        static Matrix4 Identity() { return Matrix4(); }
        static Matrix4 Translate(T x, T y, T z) {
            Matrix4 result(glm::translate(glm::mat4(1.0F), glm::vec3(x, y, z)));
            return result;
        }

        static Matrix4 Translate(Vector3<T> translation) {
            return Translate(translation.x, translation.y, translation.z);
        }

        static Matrix4 Rotate(Quat<T> quaternion) {
            Matrix4 result(glm::mat4_cast(quaternion.ToGlm()));
            return result;
        }

        static Matrix4 Scale(T x, T y, T z) {
            Matrix4 result(glm::scale(glm::mat4(1.0F), glm::vec3(x, y, z)));
            return result;
        }

        static Matrix4 Scale(Vector3<T> scale) { return Scale(scale.x, scale.y, scale.z); }

        static Matrix4 Perspective(T near, T far, T fov, T aspectRatio) {
            Matrix4 result(glm::perspective(glm::radians(fov), aspectRatio, near, far));
            return result;
        }

        static Matrix4 Orthographic(T near, T far, T down, T up, T left, T right) {
            Matrix4 result(glm::ortho(left, right, down, up, near, far));
            return result;
        }

        PIXF_MATH_GLM_OVERLOADS(Matrix4, Matrix4)

        Vector4<T> operator*(const Vector4<T> &v) const {
            glm::vec<4, T> vec = ToGlm() * v.ToGlm();
            return Vector4<T>(vec.x, vec.y, vec.z, vec.w);
        }

        Vector4<T> operator/(const Vector4<T> &v) const {
            glm::vec<4, T> vec = ToGlm() / v.ToGlm();
            return Vector4<T>(vec.x, vec.y, vec.z, vec.w);
        }

        Matrix4 &Inverse() {
            *this = Matrix4(glm::inverse(ToGlm()));
            return *this;
        }

        T *Data() {
            thread_local T buffer[16];
            ToColumnMajorArray(buffer);
            return buffer;
        }

        glm::mat<4, 4, T> ToGlm() const {
            glm::mat<4, 4, T> result;
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    result[col][row] = elements[row][col];
                }
            }

            return result;
        }

        void FromGlm(glm::mat<4, 4, T> glm) {
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    elements[row][col] = glm[col][row];
                }
            }
        }

    private:
        void ToColumnMajorArray(T out[16]) const {
            int index = 0;
            for (int col = 0; col < 4; col++) {
                for (int row = 0; row < 4; row++) {
                    out[index++] = elements[row][col];
                }
            }
        }
    };

    using Vector2f = Vector2<float>;
    using Vector3f = Vector3<float>;
    using Vector4f = Vector4<float>;

    using Vector2d = Vector2<double>;
    using Vector3d = Vector3<double>;
    using Vector4d = Vector4<double>;

    using Vector2i = Vector2<int>;
    using Vector3i = Vector3<int>;
    using Vector4i = Vector4<int>;

    using Vector2u = Vector2<unsigned>;
    using Vector3u = Vector3<unsigned>;
    using Vector4u = Vector4<unsigned>;

    using Color3u8 = Color3<uint8_t>;
    using Color4u8 = Color4<uint16_t>;

    using Color3u16 = Color3<uint8_t>;
    using Color4u16 = Color4<uint16_t>;

    using Quaternion = Quat<float>;

    using Matrix4f = Matrix4<float>;
    using Matrix4d = Matrix4<double>;
    using Matrix4i = Matrix4<int>;
    using Matrix4u = Matrix4<unsigned>;
} // namespace Pixf::Core::Math

#endif // MATH_HPP
