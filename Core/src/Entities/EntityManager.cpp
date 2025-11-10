#include "EntityManager.hpp"

#include <optional>
#include <vector>

#include "ComponentManager.hpp"

namespace Pixf::Core::Entities {
    Entity EntityManager::CreateEntity() {
        // Steal an existing inactive entity's ID
        if (const auto inactiveEntity = GetFirstInactiveEntity(); inactiveEntity.has_value()) {
            Entity &entity = *inactiveEntity.value();
            entity.version++;
            entity.active = true;
            return entity;
        }

        // Create a new entity if no inactive entities
        size_t id = GenEntityId();
        const Entity entity(id);
        if (id > m_Entities.size()) {
            m_Entities.resize(id);
        }

        m_Entities[id - 1] = entity;
        return entity;
    }

    std::optional<Entity> EntityManager::GetEntity(unsigned int id) const {
        if (id > m_Entities.size()) {
            return std::nullopt;
        }

        return m_Entities[id - 1];
    }

    void EntityManager::DestroyEntity(const Entity entity) {
        for (auto &e: m_Entities) {
            if (e.id == entity.id) {
                ClearComponents(e);
                e.active = false;
            }
        }
    }

    void EntityManager::Clear() {
        m_Entities.clear();
        m_ComponentManager.Clear();
    }

    size_t EntityManager::GenEntityId() {
        m_EntityIdCounter++;
        return m_EntityIdCounter;
    }

    std::optional<Entity *> EntityManager::GetFirstInactiveEntity() {
        for (size_t i = 0; i < m_Entities.size(); i++) {
            if (!m_Entities[i].active) {
                return &m_Entities[i];
            }
        }

        return std::nullopt;
    }
} // namespace Pixf::Core::Entities
