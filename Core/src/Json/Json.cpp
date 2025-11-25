#include "Json.hpp"

namespace Pixf::Core::Json {
    static constexpr unsigned int s_IndentLevel = 4;

    Json Json::Parse(std::string str) {
        Json json;
        json.m_Json = nlohmann::json::parse(str);
        return json;
    }

    std::string Json::ToString() const { return m_Json.dump(s_IndentLevel); }

    bool Json::Contains(const std::string &key) const { return m_Json.contains(key); }

    Json::Json(const nlohmann::ordered_json &json) : m_Json(json) {}

    Json::operator nlohmann::basic_json<nlohmann::ordered_map>() const {
        return m_Json;
    }
} // namespace Pixf::Core::Json
