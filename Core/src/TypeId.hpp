#ifndef PIXF_TYPEID_HPP
#define PIXF_TYPEID_HPP

#include "Common.hpp"

namespace Pixf::Core {
    using TypeId = uint64_t;

    TypeId PIXF_API RegisterType(const char* typeName);

    template<typename T>
    TypeId GetTypeId() {
        static const TypeId s_Id = RegisterType(typeid(T).name());
        return s_Id;
    }
} // namespace Pixf::Core

#endif // PIXF_TYPEID_HPP
