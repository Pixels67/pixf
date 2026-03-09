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
        T type{};
        return Reflect(type).name;
    }

    template<typename T>
    concept IsReflectable = requires(T type) {
        Reflect(type);
    };
}

#endif //FLK_REFLECT_HPP
