#ifndef PIXF_WORLD_HPP
#define PIXF_WORLD_HPP

#include "Registry.hpp"
#include "SystemRegistry.hpp"

namespace Pixf::Core::Entities {
    struct World {
        Registry registry;
        SystemRegistry systemRegistry;
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_WORLD_HPP
