#ifndef FLK_ASSETHANDLE_HPP
#define FLK_ASSETHANDLE_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Asset {
    using AssetId = u32;

    template<typename T>
    struct AssetHandle {
        AssetId     id       = FLK_INVALID;
        bool        resolved = false;
        std::string filePath;

        static AssetHandle FromPath(const std::string &filePath) {
            return {.filePath = filePath};
        }

        [[nodiscard]] bool IsValid() const {
            return id != FLK_INVALID;
        }

        bool operator==(const AssetHandle &assetHandle) const = default;
        bool operator!=(const AssetHandle &assetHandle) const = default;
    };

    template<typename T>
    const char *NameOf(const AssetHandle<T> &) {
        return "AssetHandle";
    }

    template<typename T>
    bool Archive(Serial::IArchive &archive, AssetHandle<T> &handle) {
        if (!archive("filePath", handle.filePath)) {
            return false;
        }

        return true;
    }
}

#endif //FLK_ASSETHANDLE_HPP
