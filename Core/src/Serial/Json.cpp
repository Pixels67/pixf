#include "Json.hpp"

#include <nlohmann/json.hpp>

namespace Flock::Serial {
    static constexpr u32 s_IndentLevel = 2;

    std::optional<Json> Json::Parse(const std::string &json) {
        try {
            return Json(nlohmann::ordered_json::parse(json));
        } catch (nlohmann::ordered_json::exception &e) {
            Debug::LogErr("Json::Parse: {}", e.what());
            return std::nullopt;
        }
    }

    std::string Json::ToString() const {
        return m_Data.dump(s_IndentLevel);
    }

    const nlohmann::ordered_json &Json::Raw() const {
        return m_Data;
    }
}
