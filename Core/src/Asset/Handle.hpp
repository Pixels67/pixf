#ifndef FLK_HANDLE_HPP
#define FLK_HANDLE_HPP

#include "Common.hpp"
#include "TypeId.hpp"

namespace Flock::Asset {
    using AssetId = u32;

    /**
     * @struct Handle
     * @tparam T The asset type.
     */
    template<typename T>
    struct FLK_API Handle {
        AssetId id;
        TypeId  typeId;
    };
}

#endif //FLK_HANDLE_HPP