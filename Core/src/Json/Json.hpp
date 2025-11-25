#ifndef PIXF_JSON_HPP
#define PIXF_JSON_HPP

#include <nlohmann/json.hpp>

#include "Common.hpp"
#include "Error/Error.hpp"

namespace Pixf::Core::Json {
    class PIXF_API JsonError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API Json {
    public:
        Json() = default;

        static Json Parse(std::string str);
        std::string ToString() const;

        bool Contains(const std::string &key) const;

        // Add trivial value
        template<typename T>
        void Add(const std::string &key, const T &value) {
            m_Json[key] = value;
        }

        // Add JSON object
        void Add(const std::string &key, const Json &value) { m_Json[key] = value.m_Json; }

        // Add JSON array
        template<typename T>
        void Add(const std::string &key, const std::vector<T> &array) {
            if constexpr (std::is_same_v<T, Json>) {
                nlohmann::json::array_t jsonArray;
                for (const Json &val: array) {
                    jsonArray.push_back(nlohmann::ordered_json(val));
                }

                m_Json[key] = jsonArray;
            } else {
                nlohmann::json::array_t jsonArray;
                for (const T &val: array) {
                    jsonArray.push_back(val);
                }

                m_Json[key] = jsonArray;
            }
        }

        // Get trivial value
        template<typename T>
        T Get(const std::string &key) const {
            if (!m_Json.contains(key)) {
                throw JsonError("Key not found: " + key);
            }

            return m_Json[key].get<T>();
        }

        // Get trivial value by reference
        template<typename T>
        T &GetRef(const std::string &key) {
            if (!m_Json.contains(key)) {
                throw JsonError("Key not found: " + key);
            }

            if (m_Json[key].is_object()) {
                throw JsonError("Cannot get object by reference: " + key);
            }

            if (m_Json[key].is_array()) {
                throw JsonError("Cannot get array by reference: " + key);
            }

            return m_Json[key].get_ref<T &>();
        }

        // Get JSON object
        Json GetObject(const std::string &key) const {
            if (!m_Json.contains(key)) {
                throw JsonError("Key not found: " + key);
            }

            if (!m_Json[key].is_object()) {
                throw JsonError("Key is not an object: " + key);
            }

            return Json(m_Json[key]);
        }

        // Get JSON array
        template<typename T>
        std::vector<T> GetArray(const std::string &key) const {
            if (!m_Json.contains(key)) {
                throw JsonError("Key not found: " + key);
            }

            if (!m_Json[key].is_array()) {
                throw JsonError("Key is not an array: " + key);
            }

            if constexpr (std::is_same_v<T, Json>) {
                auto vec = m_Json[key].get<std::vector<nlohmann::ordered_json>>();
                std::vector<Json> output;
                for (auto json : vec) {
                    output.push_back(Json(json));
                }

                return output;
            } else {
                return m_Json[key].get<std::vector<T>>();
            }
        }

    private:
        nlohmann::ordered_json m_Json;

        Json(const nlohmann::ordered_json &json);
        operator nlohmann::ordered_json() const;
    };
} // namespace Pixf::Core::Json

#endif // PIXF_JSON_HPP
