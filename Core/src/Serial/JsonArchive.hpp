#ifndef PIXF_JSONARCHIVE_HPP
#define PIXF_JSONARCHIVE_HPP

#include "Common.hpp"
#include "Json/Json.hpp"
#include "Serial.hpp"

namespace Pixf::Core::Serial {
    class PIXF_API JsonOutputArchive {
    public:
        static bool IsOutput();
        static bool IsInput();

        Json::Json Get() const;

        void Recurse(const std::string &key, const std::function<void(JsonOutputArchive &)> &func);

        template<typename T>
        bool operator()(const std::string &key, T &value) {
            if constexpr (Serializable<JsonOutputArchive, T>) {
                Recurse(key, [&](JsonOutputArchive &ar) { Serialize(ar, value); });
            } else {
                m_Json.Add(key, value);
            }

            return true;
        }

        template<typename T>
        bool operator()(const std::string &key, const std::vector<T> &array) {
            if constexpr (Serializable<JsonOutputArchive, T>) {
                std::vector<Json::Json> jsonArray;
                for (size_t i = 0; i < array.size(); i++) {
                    JsonOutputArchive inner;
                    Serialize(inner, const_cast<T &>(array[i]));
                    jsonArray.push_back(inner.Get());
                }

                m_Json.Add(key, jsonArray);
            } else {
                m_Json.Add(key, array);
            }

            return true;
        }

        template<typename T>
        bool AddArray(const std::string &key,
                      const std::vector<T> &array,
                      std::function<void(JsonOutputArchive &, T &)> func) {
            std::vector<Json::Json> jsonArray;
            for (size_t i = 0; i < array.size(); i++) {
                JsonOutputArchive inner;
                func(inner, const_cast<T &>(array[i]));
                jsonArray.push_back(inner.Get());
            }

            m_Json.Add(key, jsonArray);

            return true;
        }

    private:
        Json::Json m_Json;
    };

    class PIXF_API JsonInputArchive {
    public:
        static bool IsOutput();
        static bool IsInput();

        explicit JsonInputArchive(const Json::Json &json);

        void Recurse(const std::string &key, const std::function<void(JsonInputArchive &)> &func) const;

        template<typename T>
        bool operator()(const std::string &key, T &value) {
            if (!m_Json.Contains(key)) {
                PIXF_CORE_LOG_ERROR("Failed to deserialize key value pair: Key {} not found", key);
                return false;
            }

            if constexpr (Serializable<JsonInputArchive, T>) {
                Recurse(key, [&](JsonInputArchive &ar) { Serialize(ar, value); });
            } else {
                value = m_Json.Get<T>(key);
            }

            return true;
        }

        template<typename T>
        bool operator()(const std::string &key, std::vector<T> &array) {
            if (!m_Json.Contains(key)) {
                PIXF_CORE_LOG_ERROR("Failed to deserialize key value pair: Key {} not found", key);
                return false;
            }

            if constexpr (Serializable<JsonInputArchive, T>) {
                const std::vector<Json::Json> jsonArray = m_Json.GetArray<Json::Json>(key);
                array.clear();

                for (size_t i = 0; i < jsonArray.size(); i++) {
                    T item;
                    JsonInputArchive itemAr{jsonArray[i]};
                    Serialize(itemAr, item);
                    array.push_back(std::move(item));
                }
            } else {
                array = m_Json.GetArray<T>(key);
            }

            return true;
        }

        template<typename T>
        bool
        AddArray(const std::string &key, std::vector<T> &array, std::function<void(JsonInputArchive &, T &)> func) {
            if (!m_Json.Contains(key)) {
                PIXF_CORE_LOG_ERROR("Failed to deserialize key value pair: Key {} not found", key);
                return false;
            }

            const std::vector<Json::Json> jsonArray = m_Json.GetArray<Json::Json>(key);
            array.clear();

            for (size_t i = 0; i < jsonArray.size(); i++) {
                T item;
                JsonInputArchive itemAr{jsonArray[i]};
                func(itemAr, item);
                array.push_back(std::move(item));
            }

            return true;
        }

    private:
        Json::Json m_Json;
    };
} // namespace Pixf::Core::Serial

#endif // PIXF_JSONARCHIVE_HPP
