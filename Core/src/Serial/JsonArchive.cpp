#include "JsonArchive.hpp"

namespace Flock::Serial {
    JsonWriter::JsonWriter() {
        m_Stack.push_back(&m_Root);
    }

    Json JsonWriter::GetOutput() const {
        return Json(m_Root);
    }

    void JsonWriter::operator()(std::string_view key, bool &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, u32 &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, u64 &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, i32 &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, i64 &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, f32 &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, f64 &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, char &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, std::string &value) {
        Current()[key] = value;
    }

    void JsonWriter::operator()(std::string_view key, Vector2f &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Vector3f &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        obj["z"]       = value.z;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Vector4f &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        obj["z"]       = value.z;
        obj["w"]       = value.w;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Vector2i &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Vector3i &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        obj["z"]       = value.z;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Vector4i &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        obj["z"]       = value.z;
        obj["w"]       = value.w;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Color3u8 &value) {
        nlohmann::ordered_json obj;
        obj["r"]       = value.r;
        obj["g"]       = value.g;
        obj["b"]       = value.b;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Color4u8 &value) {
        nlohmann::ordered_json obj;
        obj["r"]       = value.r;
        obj["g"]       = value.g;
        obj["b"]       = value.b;
        obj["a"]       = value.a;
        Current()[key] = obj;
    }

    void JsonWriter::operator()(std::string_view key, Quaternion &value) {
        nlohmann::ordered_json obj;
        obj["x"]       = value.x;
        obj["y"]       = value.y;
        obj["z"]       = value.z;
        obj["w"]       = value.w;
        Current()[key] = obj;
    }

    void JsonWriter::BeginObject() {
        Current().push_back(nlohmann::ordered_json{});
        m_Stack.push_back(&Current().back());
    }

    void JsonWriter::BeginObject(std::string_view key) {
        Current()[key] = nlohmann::ordered_json{};
        m_Stack.push_back(&Current()[key]);
    }

    void JsonWriter::BeginArray(std::string_view key, usize &size) {
        Current()[key] = nlohmann::ordered_json::array();
        m_Stack.push_back(&Current()[key]);
    }

    void JsonWriter::EndObject() {
        m_Stack.pop_back();
    }

    void JsonWriter::EndArray() {
        m_Stack.pop_back();
    }

    JsonReader::JsonReader(const Json &data) : m_Root(data.Raw()) {
        m_Stack.push_back(&m_Root);
    }

    void JsonReader::operator()(std::string_view key, bool &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, u32 &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, u64 &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, i32 &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, i64 &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, f32 &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, f64 &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, char &value) {
        value = Current()[key].get<char>();
    }

    void JsonReader::operator()(std::string_view key, std::string &value) {
        value = Current()[key];
    }

    void JsonReader::operator()(std::string_view key, Vector2f &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
    }

    void JsonReader::operator()(std::string_view key, Vector3f &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
        value.z            = obj["z"];
    }

    void JsonReader::operator()(std::string_view key, Vector4f &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
        value.z            = obj["z"];
        value.w            = obj["w"];
    }

    void JsonReader::operator()(std::string_view key, Vector2i &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
    }

    void JsonReader::operator()(std::string_view key, Vector3i &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
        value.z            = obj["z"];
    }

    void JsonReader::operator()(std::string_view key, Vector4i &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
        value.z            = obj["z"];
        value.w            = obj["w"];
    }

    void JsonReader::operator()(std::string_view key, Color3u8 &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.r            = obj["r"];
        value.g            = obj["g"];
        value.b            = obj["b"];
    }

    void JsonReader::operator()(std::string_view key, Color4u8 &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.r            = obj["r"];
        value.g            = obj["g"];
        value.b            = obj["b"];
        value.a            = obj["a"];
    }

    void JsonReader::operator()(std::string_view key, Quaternion &value) {
        nlohmann::ordered_json obj = Current()[key];
        value.x            = obj["x"];
        value.y            = obj["y"];
        value.z            = obj["z"];
        value.w            = obj["w"];
    }

    void JsonReader::BeginObject() {
        const usize idx = m_IndexStack.back()++;
        m_Stack.push_back(&Current()[idx]);
    }

    void JsonReader::BeginObject(std::string_view key) {
        m_Stack.push_back(&Current()[key]);
    }

    void JsonReader::BeginArray(std::string_view key, usize &size) {
        m_Stack.push_back(&Current()[key]);
        m_IndexStack.push_back(0);
        size = Current().size();
    }

    void JsonReader::EndObject() {
        m_Stack.pop_back();
    }

    void JsonReader::EndArray() {
        m_Stack.pop_back();
        m_IndexStack.pop_back();
    }
}
