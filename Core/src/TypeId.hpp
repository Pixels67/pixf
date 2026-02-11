#ifndef FLK_TYPEID_HPP
#define FLK_TYPEID_HPP

#include "Common.hpp"

namespace Flock {
    using TypeId = u64;

    TypeId FLK_API RegisterType(const char* typeName);

    template<typename T>
    TypeId GetTypeId() {
        static const TypeId s_Id = RegisterType(typeid(T).name());
        return s_Id;
    }
}

#endif //FLK_TYPEID_HPP