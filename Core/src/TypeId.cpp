#include "TypeId.hpp"

namespace Pixf::Core {
    static std::unordered_map<std::string, TypeId> s_TypeRegistry;
    static TypeId s_NextId = 1;

    TypeId RegisterType(const char* typeName) {
        const std::string name(typeName);

        if (s_TypeRegistry.contains(name)) {
            return s_TypeRegistry.at(name);
        }

        const TypeId newId = s_NextId++;
        s_TypeRegistry[name] = newId;

        return newId;
    }
} // namespace Pixf::Core
