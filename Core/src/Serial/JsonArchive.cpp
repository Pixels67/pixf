#include "JsonArchive.hpp"

#include "Debug/Log.hpp"
#include "Math/Color.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector.hpp"
#include "Serial/Json.hpp"

namespace Flock::Serial {
    JsonWriter::JsonWriter() {
        m_Stack.push_back(&m_Root);
    }

    Json JsonWriter::Output() const {
        return Json(m_Root);
    }

    bool JsonWriter::operator()(std::string_view key, bool &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, u32 &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, u64 &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, i32 &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, i64 &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, f32 &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, f64 &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, char &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, std::string &value) {
        try {
            Current()[key] = value;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Vector2f &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Vector3f &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            obj["z"]       = value.z;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Vector4f &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            obj["z"]       = value.z;
            obj["w"]       = value.w;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Vector2i &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Vector3i &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            obj["z"]       = value.z;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Vector4i &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            obj["z"]       = value.z;
            obj["w"]       = value.w;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Color3u8 &value) {
        try {
            nlohmann::ordered_json obj;
            obj["r"]       = value.r;
            obj["g"]       = value.g;
            obj["b"]       = value.b;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Color4u8 &value) {
        try {
            nlohmann::ordered_json obj;
            obj["r"]       = value.r;
            obj["g"]       = value.g;
            obj["b"]       = value.b;
            obj["a"]       = value.a;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonWriter::operator()(std::string_view key, Quaternion &value) {
        try {
            nlohmann::ordered_json obj;
            obj["x"]       = value.x;
            obj["y"]       = value.y;
            obj["z"]       = value.z;
            obj["w"]       = value.w;
            Current()[key] = obj;
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
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
        try {
            m_Stack.push_back(&m_Root);
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
        }
    }

    bool JsonReader::operator()(std::string_view key, bool &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, u32 &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, u64 &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, i32 &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, i64 &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, f32 &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, f64 &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, char &value) {
        try {
            value = Current()[key].get<char>();
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, std::string &value) {
        try {
            value = Current()[key];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Vector2f &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Vector3f &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
            value.z                    = obj["z"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Vector4f &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
            value.z                    = obj["z"];
            value.w                    = obj["w"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Vector2i &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Vector3i &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
            value.z                    = obj["z"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Vector4i &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
            value.z                    = obj["z"];
            value.w                    = obj["w"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Color3u8 &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.r                    = obj["r"];
            value.g                    = obj["g"];
            value.b                    = obj["b"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Color4u8 &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.r                    = obj["r"];
            value.g                    = obj["g"];
            value.b                    = obj["b"];
            value.a                    = obj["a"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    bool JsonReader::operator()(std::string_view key, Quaternion &value) {
        try {
            nlohmann::ordered_json obj = Current()[key];
            value.x                    = obj["x"];
            value.y                    = obj["y"];
            value.z                    = obj["z"];
            value.w                    = obj["w"];
        } catch (const nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("JsonReader: {}", str);
            return false;
        }

        return true;
    }

    void JsonReader::BeginObject() {
        const usize idx = m_IndexStack.back()++;
        m_Stack.push_back(&Current()[idx]);
    }

    void JsonReader::BeginObject(std::string_view key) {
        m_Stack.push_back(&Current()[key]);
    }

    void JsonReader::BeginArray(std::string_view key, usize &size) {
        if (key == "messages") {

        }

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
