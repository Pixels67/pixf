#include "AssetLoader.hpp"

namespace Flock::Asset {
    void AssetLoader::UnloadAll() {
        for (auto &asset: m_Assets) {
            if (!asset.has_value()) {
                continue;
            }

            asset = std::nullopt;
        }
    }
}
