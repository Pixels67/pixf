#include "Json.hpp"

namespace Pixf::Core::Json {
    static constexpr unsigned int s_IndentLevel = 4;

    Array::Array(const nlohmann::json::array_t &array) : m_Array(array) {}

    void Array::PushObject(const Json &object) { m_Array.push_back(object.m_Json); }

    size_t Array::Size() const { return m_Array.size(); }

    std::string Json::ToString() const { return m_Json.dump(s_IndentLevel); }

    void Json::AddObject(const std::string &key, std::function<void(Json &)> &&func) {
        const auto fn = std::move(func);
        Json obj;
        fn(obj);
        Add(key, obj.m_Json);
    }

    void Json::AddObject(const std::string &key, const Json &object) { Add(key, object.m_Json); }

    Json Json::GetObject(const std::string &key) const { return m_Json.at(key); }

    void Json::AddArray(const std::string &key, const Array &array) { Add(key, array.m_Array); }

    void Json::AddArray(const std::string &key, std::function<void(Array &)> &&func) {
        const auto fn = std::move(func);
        Array array;
        fn(array);
        Add(key, array.m_Array);
    }

    Array Json::GetArray(const std::string &key) const {
        return Array(static_cast<nlohmann::json::array_t>(m_Json.at(key)));
    }

    bool Json::Contains(const std::string &key) const { return m_Json.contains(key); }

    Json::Json(const nlohmann::ordered_json &json) : m_Json(json) {}

    Json Json::Parse(std::string str) {
        Json json;
        json.m_Json = nlohmann::json::parse(str);
        return json;
    }
} // namespace Pixf::Core::Json
