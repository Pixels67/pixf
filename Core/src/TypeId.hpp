#ifndef PIXF_TYPEID_HPP
#define PIXF_TYPEID_HPP

#include "Common.hpp"

namespace Pixf::Core {
    using TypeId = uint64_t;

    TypeId PIXF_API GetNextTypeId();

    template<typename T>
    TypeId GetTypeId() {
        static const TypeId s_Id = GetNextTypeId();
        return s_Id;
    }
}

#endif // PIXF_TYPEID_HPP
