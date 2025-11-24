#ifndef PIXF_JSON_HPP
#define PIXF_JSON_HPP

#include <nlohmann/json.hpp>

#include "Common.hpp"

namespace Pixf::Core::Json {
    class Json;

    class PIXF_API Array {
    public:
        friend class Json;

        Array() = default;

        void PushObject(const Json &object);

        size_t Size() const;

        template<typename T>
        void Push(const T &value) {
            m_Array.push_back(value);
        }

        template<typename T>
        void Pop() {
            m_Array.pop_back();
        }

        template<typename T>
        T At(const unsigned int index) {
            return m_Array.at(index);
        }

    private:
        nlohmann::json::array_t m_Array;

        Array(const nlohmann::json::array_t &array);
    };

    class PIXF_API Json {
    public:
        friend class Array;

        Json() = default;

        static Json Parse(std::string str);

        std::string ToString() const;

        void AddObject(const std::string &key, const Json &object);
        void AddObject(const std::string &key, std::function<void(Json &)> &&func);
        Json GetObject(const std::string &key) const;

        void AddArray(const std::string &key, const Array &array);
        void AddArray(const std::string &key, std::function<void(Array &)> &&func);
        Array GetArray(const std::string &key) const;

        bool Contains(const std::string &key) const;

        template<typename T>
        void Add(const std::string &key, const T &value) {
            m_Json[key] = value;
        }

        template<typename T>
        T Get(const std::string &key) const {
            return static_cast<T>(m_Json[key].get<T>());
        }

        template<typename T>
        T &GetRef(const std::string &key) {
            return static_cast<T&>(m_Json[key].get_ref<T&>());
        }

    private:
        nlohmann::ordered_json m_Json;

        Json(const nlohmann::ordered_json &json);
    };
} // namespace Pixf::Core::Json

#endif // PIXF_JSON_HPP
