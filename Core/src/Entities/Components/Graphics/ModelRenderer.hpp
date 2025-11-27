#ifndef PIXF_MODEL_HPP
#define PIXF_MODEL_HPP

#include "Serial/Serial.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Entities::Components::Graphics {
    struct ModelRenderer {
        Files::Assets::ModelAssetHandle assetHandle;
    };

    PIXF_SERIALIZE(ModelRenderer, Serialize(archive, ModelRenderer.assetHandle.uuid);)
} // namespace Pixf::Core::Entities::Graphics

#endif // PIXF_MODEL_HPP
