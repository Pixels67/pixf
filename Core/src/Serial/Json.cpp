#include "Json.hpp"

#include <nlohmann/json.hpp>

#include "Common.hpp"

namespace Flock::Serial {
    static constexpr u32 s_IndentLevel = 2;

    std::optional<Json> Json::Parse(const std::string &json) {
        try {
            return Json(nlohmann::ordered_json::parse(json));
        } catch (nlohmann::ordered_json::exception &e) {
            std::string str = e.what();
            str             = str.substr(str.find_first_of(']') + 2);
            Debug::LogErr("Json::Parse: {}", str);
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
