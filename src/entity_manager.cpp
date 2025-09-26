#include "entity_manager.h"

namespace Engine {
    unsigned int EntityManager::CreateEntity() {
        unsigned int id = GenerateEntityId();
        Entity entity;
        entity.id = id;
        m_Entities.push_back(entity);
        return id;
    }

    unsigned int EntityManager::GenerateEntityId() {
        return m_Entities.size();
    }
} // namespace Engine
