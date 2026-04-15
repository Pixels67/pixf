#ifndef FLK_ARCHIVE_HPP
#define FLK_ARCHIVE_HPP

#include <map>

#include "Common.hpp"

namespace Flock {
    template<typename T>
    struct Vector2;

    template<typename T>
    struct Vector3;

    template<typename T>
    struct Vector4;

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

    template<typename T>
    struct Color3;

    template<typename T>
    struct Color4;

    using Color3u8 = Color3<u8>;
    using Color4u8 = Color4<u8>;

    struct Quaternion;
}

namespace Flock::Serial {
    class IArchive;

    template<class T>
    concept Serializable =
            requires(const std::remove_cvref_t<T> &t) {
                { NameOf(t) } -> std::convertible_to<const char *>;
            } &&
            requires(IArchive &ar, std::remove_cvref_t<T> v) {
                { Archive(ar, v) } -> std::same_as<bool>;
            };

    class FLK_API IArchive {
    public:
        virtual ~IArchive() = default;

        template<Serializable T>
        bool operator()(const std::string_view key, T &value) {
            BeginObject(key);
            if (!Archive(*this, value)) {
                EndObject();
                Debug::LogErr("IArchive: Failed to archive '{}'", key);
                return false;
            }

            EndObject();
            return true;
        }

        template<typename T>
        bool operator()(const std::string_view key, std::vector<T> &value) {
            usize size = value.size();
            BeginArray(key, size);
            value.resize(size);

            bool success = true;

            i32 idx = 0;
            for (T &elem: value) {
                BeginObject();
                if (!(*this)("value", elem)) {
                    EndObject();
                    success = false;
                    Debug::LogErr("IArchive: Failed to archive array element '{}[{}]'", key, idx);
                    continue;
                }

                EndObject();
                idx++;
            }

            EndArray();
            return success;
        }

        template<typename K, typename V>
        requires std::convertible_to<K, std::string>
        bool operator()(const std::string_view key, std::map<K, V> &value) {
            bool success = true;
            BeginObject(key);

            for (auto &[k, val]: value) {
                if (!(*this)(std::string(k), val)) {
                    success = false;
                    Debug::LogErr("IArchive: Failed to archive map element '{}[{}]'", key, k);
                }
            }

            EndObject();
            return success;
        }

        template<typename K, typename V>
        requires std::convertible_to<K, std::string>
        bool operator()(const std::string_view key, std::unordered_map<K, V> &value) {
            bool success = true;
            BeginObject(key);

            for (auto &[k, val]: value) {
                if (!(*this)(std::string(k), val)) {
                    success = false;
                    Debug::LogErr("IArchive: Failed to archive map element '{}[{}]'", key, k);
                }
            }

            EndObject();
            return success;
        }

        template<typename T> requires std::is_enum_v<T>
        bool operator()(const std::string_view key, T &value) {
            auto underlying = static_cast<u64>(value);
            (*this)(key, underlying);
            value = static_cast<T>(underlying);

            return true;
        }

        virtual usize CurrentArraySize() = 0;

        virtual bool operator()(std::string_view, bool &) = 0;

        virtual bool operator()(std::string_view, u32 &) = 0;
        virtual bool operator()(std::string_view, u64 &) = 0;

        virtual bool operator()(std::string_view, i32 &) = 0;
        virtual bool operator()(std::string_view, i64 &) = 0;

        virtual bool operator()(std::string_view, f32 &) = 0;
        virtual bool operator()(std::string_view, f64 &) = 0;

        virtual bool operator()(std::string_view, char &) = 0;
        virtual bool operator()(std::string_view, std::string &) = 0;

        virtual bool operator()(std::string_view, Vector2f &) = 0;
        virtual bool operator()(std::string_view, Vector3f &) = 0;
        virtual bool operator()(std::string_view, Vector4f &) = 0;

        virtual bool operator()(std::string_view, Vector2i &) = 0;
        virtual bool operator()(std::string_view, Vector3i &) = 0;
        virtual bool operator()(std::string_view, Vector4i &) = 0;

        virtual bool operator()(std::string_view, Color3u8 &) = 0;
        virtual bool operator()(std::string_view, Color4u8 &) = 0;

        virtual bool operator()(std::string_view, Quaternion &) = 0;

        virtual void BeginObject() = 0;
        virtual void BeginObject(std::string_view key) = 0;

        virtual void BeginArray(std::string_view key, usize &) = 0;

        virtual void EndObject() = 0;
        virtual void EndArray() = 0;
    };
}

#define FLK_ARCHIVE_TAG(type, ...) \
    inline const char *NameOf(const type &) { return #type; } \
    inline bool Archive(Flock::Serial::IArchive &ar, type &val) { return true; }

#define FLK_ARCHIVE(type, ...) \
    inline const char *NameOf(const type &) { return #type; } \
    inline bool Archive(Flock::Serial::IArchive &ar, type &val) { \
        bool success = true; \
        FLK_ARCHIVE_FIELDS(__VA_ARGS__); \
        return success; \
    }

#define FLK_FIELD(field) if (!ar(#field, val.field)) success = false

// We do not talk about the triangle...
#define FLK_FIELDS_1(a) FLK_FIELD(a)
#define FLK_FIELDS_2(a, b) FLK_FIELD(a); FLK_FIELD(b)
#define FLK_FIELDS_3(a, b, c) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c)
#define FLK_FIELDS_4(a, b, c, d) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d)
#define FLK_FIELDS_5(a, b, c, d, e) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e)
#define FLK_FIELDS_6(a, b, c, d, e, f) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f)
#define FLK_FIELDS_7(a, b, c, d, e, f, g) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g)
#define FLK_FIELDS_8(a, b, c, d, e, f, g, h) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h)
#define FLK_FIELDS_9(a, b, c, d, e, f, g, h, i) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i)
#define FLK_FIELDS_10(a, b, c, d, e, f, g, h, i, j) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j)
#define FLK_FIELDS_11(a, b, c, d, e, f, g, h, i, j, k) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k)
#define FLK_FIELDS_12(a, b, c, d, e, f, g, h, i, j, k, l) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l)
#define FLK_FIELDS_13(a, b, c, d, e, f, g, h, i, j, k, l, m) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m)
#define FLK_FIELDS_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n)
#define FLK_FIELDS_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o)
#define FLK_FIELDS_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p)
#define FLK_FIELDS_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q)
#define FLK_FIELDS_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r)
#define FLK_FIELDS_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s)
#define FLK_FIELDS_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t)
#define FLK_FIELDS_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u)
#define FLK_FIELDS_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v)
#define FLK_FIELDS_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w)
#define FLK_FIELDS_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x)
#define FLK_FIELDS_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y)
#define FLK_FIELDS_26(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z)
#define FLK_FIELDS_27(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z); FLK_FIELD(aa)
#define FLK_FIELDS_28(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z); FLK_FIELD(aa); FLK_FIELD(ab)
#define FLK_FIELDS_29(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z); FLK_FIELD(aa); FLK_FIELD(ab); FLK_FIELD(ac)
#define FLK_FIELDS_30(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z); FLK_FIELD(aa); FLK_FIELD(ab); FLK_FIELD(ac); FLK_FIELD(ad)
#define FLK_FIELDS_31(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z); FLK_FIELD(aa); FLK_FIELD(ab); FLK_FIELD(ac); FLK_FIELD(ad); FLK_FIELD(ae)
#define FLK_FIELDS_32(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af) FLK_FIELD(a); FLK_FIELD(b); FLK_FIELD(c); FLK_FIELD(d); FLK_FIELD(e); FLK_FIELD(f); FLK_FIELD(g); FLK_FIELD(h); FLK_FIELD(i); FLK_FIELD(j); FLK_FIELD(k); FLK_FIELD(l); FLK_FIELD(m); FLK_FIELD(n); FLK_FIELD(o); FLK_FIELD(p); FLK_FIELD(q); FLK_FIELD(r); FLK_FIELD(s); FLK_FIELD(t); FLK_FIELD(u); FLK_FIELD(v); FLK_FIELD(w); FLK_FIELD(x); FLK_FIELD(y); FLK_FIELD(z); FLK_FIELD(aa); FLK_FIELD(ab); FLK_FIELD(ac); FLK_FIELD(ad); FLK_FIELD(ae); FLK_FIELD(af)

#define FLK_ARCHIVE_GET_MACRO(af, ae, ad, ac, ab, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a, NAME, ...) NAME

#define FLK_EXPAND(x) x

#define FLK_ARCHIVE_FIELDS(...) \
FLK_EXPAND(FLK_ARCHIVE_GET_MACRO(__VA_ARGS__, \
    FLK_FIELDS_32, FLK_FIELDS_31, FLK_FIELDS_30, FLK_FIELDS_29, \
    FLK_FIELDS_28, FLK_FIELDS_27, FLK_FIELDS_26, FLK_FIELDS_25, \
    FLK_FIELDS_24, FLK_FIELDS_23, FLK_FIELDS_22, FLK_FIELDS_21, \
    FLK_FIELDS_20, FLK_FIELDS_19, FLK_FIELDS_18, FLK_FIELDS_17, \
    FLK_FIELDS_16, FLK_FIELDS_15, FLK_FIELDS_14, FLK_FIELDS_13, \
    FLK_FIELDS_12, FLK_FIELDS_11, FLK_FIELDS_10, FLK_FIELDS_9,  \
    FLK_FIELDS_8,  FLK_FIELDS_7,  FLK_FIELDS_6,  FLK_FIELDS_5,  \
    FLK_FIELDS_4,  FLK_FIELDS_3,  FLK_FIELDS_2,  FLK_FIELDS_1)  \
(__VA_ARGS__))

#endif //FLK_ARCHIVE_HPP
