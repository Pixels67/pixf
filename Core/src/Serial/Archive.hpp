#ifndef FLK_ARCHIVE_HPP
#define FLK_ARCHIVE_HPP

#include "Common.hpp"
#include "Math/Math.hpp"

namespace Flock::Serial {
    enum class ArchiveType {
        Input,
        Output
    };

    class IArchive;

    template<typename T>
    void Serialize(IArchive &archive, T &value);

    class FLK_API IArchive {
    public:
        ArchiveType type = ArchiveType::Output;

        virtual ~IArchive() = default;

        template<typename T> requires IsReflectable<T>
        void operator()(const std::string_view key, T &value) {
            BeginObject(key);
            Serialize(*this, value);
            EndObject();
        }

        template<typename T> requires std::is_enum_v<T>
        void operator()(std::string_view key, T &value) {
            auto underlying = static_cast<std::underlying_type_t<T>>(value);
            (*this)(key, underlying);
            value = static_cast<T>(underlying);
        }

        virtual usize CurrentArraySize() = 0;

        virtual void operator()(std::string_view, bool &) = 0;

        virtual void operator()(std::string_view, u32 &) = 0;
        virtual void operator()(std::string_view, u64 &) = 0;

        virtual void operator()(std::string_view, i32 &) = 0;
        virtual void operator()(std::string_view, i64 &) = 0;

        virtual void operator()(std::string_view, f32 &) = 0;
        virtual void operator()(std::string_view, f64 &) = 0;

        virtual void operator()(std::string_view, std::string &) = 0;

        virtual void operator()(std::string_view, Vector2f &) = 0;
        virtual void operator()(std::string_view, Vector3f &) = 0;
        virtual void operator()(std::string_view, Vector4f &) = 0;

        virtual void operator()(std::string_view, Vector2i &) = 0;
        virtual void operator()(std::string_view, Vector3i &) = 0;
        virtual void operator()(std::string_view, Vector4i &) = 0;

        virtual void operator()(std::string_view, Color3u8 &) = 0;
        virtual void operator()(std::string_view, Color4u8 &) = 0;

        virtual void operator()(std::string_view, Quaternion &) = 0;

        virtual void BeginObject() = 0;
        virtual void BeginObject(std::string_view key) = 0;

        virtual void BeginArray(std::string_view key, usize &) = 0;

        virtual void EndObject() = 0;
        virtual void EndArray() = 0;
    };

    template<typename T>
    void Serialize(IArchive &archive, T &value) {
        auto reflectable = Reflect(value);
        std::apply([&](auto &... fields) {
            (archive(fields.name, *fields.value), ...);
        }, reflectable.fields);
    }
}

#endif //FLK_ARCHIVE_HPP
