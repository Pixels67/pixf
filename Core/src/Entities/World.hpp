#ifndef PIXF_WORLD_HPP
#define PIXF_WORLD_HPP

#include "Registry.hpp"
#include "Serial/Serial.hpp"
#include "SystemRegistry.hpp"

namespace Pixf::Core::Entities {
    struct World {
        Registry registry;
        SystemRegistry systemRegistry;
    };

    PIXF_SERIALIZE(World,
        PIXF_FIELD(registry, World.registry);
    )
} // namespace Pixf::Core::Entities

#endif // PIXF_WORLD_HPP
