#ifndef FLK_ASSETHANDLE_HPP
#define FLK_ASSETHANDLE_HPP

#include "Common.hpp"
#include "TypeId.hpp"

namespace Flock::Asset {
    using AssetId = u32;

    /**
     * @struct AssetHandle
     * @tparam T The asset type.
     */
    template<typename T>
    struct FLK_API AssetHandle {
        AssetId id;
        TypeId  typeId;
    };
}

#endif //FLK_ASSETHANDLE_HPP