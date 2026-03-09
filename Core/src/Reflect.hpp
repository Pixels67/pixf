#ifndef FLK_REFLECT_HPP
#define FLK_REFLECT_HPP

#include "Common.hpp"

namespace Flock {
    template<typename T>
    struct FLK_API Field {
        std::string name;
        T *         value;

        Field(const std::string &name, T *value) : name(name), value(value) {
        }
    };

    template<typename... Fields>
    struct Reflectable {
        std::string           name;
        std::tuple<Fields...> fields;

        Reflectable(const std::string &name, std::tuple<Fields...> fields) : name(name), fields(fields) {
        }
    };

    template<typename T>
    std::string GetTypeName() {
        // Yandere-Driven Development
        if constexpr (std::same_as<T, usize>) {
            return "usize";
        } else if constexpr (std::same_as<T, isize>) {
            return "isize";
        } else if constexpr (std::same_as<T, u32>) {
            return "u32";
        } else if constexpr (std::same_as<T, u64>) {
            return "u64";
        } else if constexpr (std::same_as<T, i32>) {
            return "i32";
        } else if constexpr (std::same_as<T, i64>) {
            return "i64";
        } else if constexpr (std::same_as<T, f32>) {
            return "f32";
        } else if constexpr (std::same_as<T, f64>) {
            return "f64";
        } else if constexpr (std::same_as<T, char>) {
            return "char";
        } else if constexpr (std::same_as<T, std::string>) {
            return "string";
        } else {
            T type{};
            return Reflect(type).name;
        }
    }

    template<typename T>
    concept IsReflectable = requires(T type) {
        Reflect(type);
    };
}

#endif //FLK_REFLECT_HPP
