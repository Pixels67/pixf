#include "Registry.hpp"

namespace Pixf::Core::Entities {
    Entity Registry::CreateEntity() {
        return m_Registry.create();
    }

    void Registry::DestroyEntity(const Entity entity) {
        m_Registry.destroy(entity);
    }
}