#ifndef PIXF_MODEL_HPP
#define PIXF_MODEL_HPP

#include "Common.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Entities::Graphics {
    struct ModelRenderer {
        Uuid::Uuid uuid;
    };

    template<typename Archive>
    PIXF_API void Serialize(Archive &archive, ModelRenderer &model) {
        Serialize(archive, model.uuid);
    }
} // namespace Pixf::Core::Entities::Graphics

#endif // PIXF_MODEL_HPP
