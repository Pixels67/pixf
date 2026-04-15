#ifndef FLK_JSON_HPP
#define FLK_JSON_HPP

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>

#include "Common.hpp"
#include "Debug/Log.hpp"
#include "nlohmann/json_fwd.hpp"

namespace Flock::Serial {
    class Json {
        nlohmann::ordered_json m_Data;

    public:
        static std::optional<Json> Parse(const std::string &str);

        explicit Json(const nlohmann::ordered_json &raw) : m_Data(raw) {
        }

        template<typename T>
        void Set(std::string_view key, const T &value) {
            m_Data[key] = value;
        }

        template<typename T>
        std::optional<T> Get(std::string_view key) const {
            try {
                return m_Data.at(key).get<T>();
            } catch (const nlohmann::ordered_json::exception &e) {
                Debug::LogErr("Json::Get: {}", e.what());
                return std::nullopt;
            }
        }

        [[nodiscard]] std::string ToString() const;

        [[nodiscard]] const nlohmann::ordered_json &Raw() const;
    };
}

#endif //FLK_JSON_HPP
