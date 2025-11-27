#ifndef PIXF_ASSETHANDLE_HPP
#define PIXF_ASSETHANDLE_HPP

#include "Graphics/Model.hpp"
#include "Handle.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Files::Assets {
    template<typename T>
    struct AssetHandle {
        Uuid::Uuid uuid{};
    };

    using ModelAssetHandle = AssetHandle<Graphics::Model>;
    using Texture2DAssetHandle = AssetHandle<Graphics::Texture2DTag>;
    using ClipAssetHandle = AssetHandle<Audio::ClipTag>;
} // namespace Pixf::Core::Files::Assets

#endif // PIXF_ASSETHANDLE_HPP
