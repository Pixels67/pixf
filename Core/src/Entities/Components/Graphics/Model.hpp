#ifndef PIXF_MODEL_HPP
#define PIXF_MODEL_HPP

#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Entities::Graphics {
    struct Model {
        Uuid::Uuid uuid;
    };

    template<typename Archive>
    void Serialize(Archive &archive, Model &model) {
        archive("uuid", model.uuid);
    }
} // namespace Pixf::Core::Entities::Graphics

#endif // PIXF_MODEL_HPP
