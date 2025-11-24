#ifndef PIXF_JSONARCHIVE_HPP
#define PIXF_JSONARCHIVE_HPP

#include "Common.hpp"
#include "Json/Json.hpp"
#include "Serial.hpp"

namespace Pixf::Core::Serial {
    class PIXF_API JsonOutputArchive {
    public:
        Json::Json Get() const;

        void AddObject(const std::string &key, std::function<void(JsonOutputArchive &)> &&func);

        template<typename T>
        void operator()(const std::string &key, T &value) {
            if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, bool> || std::is_same_v<T, std::string>) {
                m_Json.Add(key, value);
            } else if constexpr (Serializable<JsonOutputArchive, T>) {
                JsonOutputArchive ar;
                T::Serialize(ar, value);
                m_Json.AddObject(key, ar.Get());
            }
        }

    private:
        Json::Json m_Json;
    };

    class PIXF_API JsonInputArchive {
    public:
        explicit JsonInputArchive(const Json::Json &json);

        void AddObject(const std::string &key, std::function<void(JsonInputArchive &)> &&func) const;

        template<typename T>
        void operator()(const std::string &key, T &value) {
            if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, bool> || std::is_same_v<T, std::string>) {
                value = m_Json.Get<T>(key);
            } else if constexpr (Serializable<JsonInputArchive, T>){
                JsonInputArchive ar(m_Json.GetObject(key));
                T::Serialize(ar, value);
            }
        }

    private:
        Json::Json m_Json;
    };
} // namespace Pixf::Core::Serial

#endif // PIXF_JSONARCHIVE_HPP
