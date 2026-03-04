#include "World.hpp"

namespace Flock::Ecs {
    Registry &World::GetRegistry() {
        return m_Registry;
    }
}
