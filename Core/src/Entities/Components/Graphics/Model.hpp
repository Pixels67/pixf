#ifndef PIXF_MODEL_HPP
#define PIXF_MODEL_HPP

#include "Serialization/Serialization.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Entities::Graphics {
    struct Model {
        Uuid::Uuid uuid;
    };

    template<typename Archive>
    void Serialize(Archive &archive, Model &model) {
        Serialization::SerializeNvp(archive, "uuid", model.uuid);
    }

    PIXF_SERIALIZE(Model)
} // namespace Pixf::Core::Entities::Graphics

#endif // PIXF_MODEL_HPP
