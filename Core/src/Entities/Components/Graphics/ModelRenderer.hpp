#ifndef PIXF_MODEL_HPP
#define PIXF_MODEL_HPP

#include "Common.hpp"
#include "Serial/Serial.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Entities::Graphics {
    struct ModelRenderer {
        Uuid::Uuid uuid;
    };

    PIXF_SERIALIZE(ModelRenderer, Serialize(archive, ModelRenderer.uuid);)
} // namespace Pixf::Core::Entities::Graphics

#endif // PIXF_MODEL_HPP
