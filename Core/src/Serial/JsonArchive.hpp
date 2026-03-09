#ifndef FLK_JSONARCHIVE_HPP
#define FLK_JSONARCHIVE_HPP

#include "Archive.hpp"
#include "Common.hpp"
#include "Json.hpp"

namespace Flock::Serial {
    class FLK_API JsonWriter : public IArchive {
        nlohmann::ordered_json                m_Root;
        std::vector<nlohmann::ordered_json *> m_Stack;

        nlohmann::ordered_json &Current() const {
            return *m_Stack.back();
        }

    public:
        JsonWriter();
        Json GetOutput() const;

        usize CurrentArraySize() override {
            return 0;
        }

        void operator()(std::string_view key, bool &value) override;

        void operator()(std::string_view key, u32 &value) override;
        void operator()(std::string_view key, u64 &value) override;

        void operator()(std::string_view key, i32 &value) override;
        void operator()(std::string_view key, i64 &value) override;

        void operator()(std::string_view key, f32 &value) override;
        void operator()(std::string_view key, f64 &value) override;

        void operator()(std::string_view key, std::string &value) override;

        void operator()(std::string_view key, Vector2f &value) override;
        void operator()(std::string_view key, Vector3f &value) override;
        void operator()(std::string_view key, Vector4f &value) override;

        void operator()(std::string_view key, Vector2i &value) override;
        void operator()(std::string_view key, Vector3i &value) override;
        void operator()(std::string_view key, Vector4i &value) override;

        void operator()(std::string_view key, Color3u8 &value) override;
        void operator()(std::string_view key, Color4u8 &value) override;

        void operator()(std::string_view key, Quaternion &value) override;

        void BeginObject() override;
        void BeginObject(std::string_view key) override;

        void BeginArray(std::string_view key, usize &size) override;

        void EndObject() override;
        void EndArray() override;
    };

    class FLK_API JsonReader : public IArchive {
        nlohmann::ordered_json                m_Root;
        std::vector<nlohmann::ordered_json *> m_Stack;
        std::vector<usize>            m_IndexStack;

        nlohmann::ordered_json &Current() const {
            return *m_Stack.back();
        }

    public:
        explicit JsonReader(const Json &data);

        usize CurrentArraySize() override {
            return Current().size();
        }

        void operator()(std::string_view key, bool &value) override;

        void operator()(std::string_view key, u32 &value) override;
        void operator()(std::string_view key, u64 &value) override;

        void operator()(std::string_view key, i32 &value) override;
        void operator()(std::string_view key, i64 &value) override;

        void operator()(std::string_view key, f32 &value) override;
        void operator()(std::string_view key, f64 &value) override;

        void operator()(std::string_view key, std::string &value) override;

        void operator()(std::string_view key, Vector2f &value) override;
        void operator()(std::string_view key, Vector3f &value) override;
        void operator()(std::string_view key, Vector4f &value) override;

        void operator()(std::string_view key, Vector2i &value) override;
        void operator()(std::string_view key, Vector3i &value) override;
        void operator()(std::string_view key, Vector4i &value) override;

        void operator()(std::string_view key, Color3u8 &value) override;
        void operator()(std::string_view key, Color4u8 &value) override;

        void operator()(std::string_view key, Quaternion &value) override;

        void BeginObject() override;
        void BeginObject(std::string_view key) override;

        void BeginArray(std::string_view key, usize &size) override;

        void EndObject() override;
        void EndArray() override;
    };
}

#endif //FLK_JSONARCHIVE_HPP
