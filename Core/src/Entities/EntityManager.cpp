#include "EntityManager.hpp"

#include <optional>
#include <vector>

#include "ComponentManager.hpp"

namespace Pixf::Core::Entities {
    Entity EntityManager::CreateEntity(const std::string &name) {
        // Steal an existing inactive entity's ID
        if (const auto inactiveEntity = GetFirstInactiveEntity(); inactiveEntity.has_value()) {
            Entity &entity = *inactiveEntity.value();
            entity.version++;
            entity.active = true;
            entity.name = name;
            return entity;
        }

        // Create a new entity if no inactive entities
        const size_t id = GenEntityId();
        const Entity entity(id, name);
        if (id > m_Entities.size()) {
            m_Entities.resize(id);
        }

        m_Entities[id - 1] = entity;
        PIXF_LOG_DEBUG("Created entity: ", entity.id);
        return entity;
    }

    std::optional<Entity> EntityManager::GetEntity(const unsigned int id) const {
        if (id > m_Entities.size()) {
            return std::nullopt;
        }

        return m_Entities[id - 1];
    }

    std::vector<Entity> EntityManager::GetAllEntities() const {
        std::vector<Entity> entities;
        for (const auto &entity: m_Entities) {
            if (entity.IsActive()) {
                entities.push_back(entity);
            }
        }

        return entities;
    }

    void EntityManager::SetName(Entity &entity, const std::string &name) {
        for (auto &e: m_Entities) {
            if (e.id == entity.id) {
                e.name = name;
                entity.name = name;
            }
        }
    }

    void EntityManager::DestroyEntity(const Entity entity) {
        for (auto &e: m_Entities) {
            if (e.id == entity.id) {
                ClearComponents(e);
                e.active = false;
                PIXF_LOG_DEBUG("Destroyed entity: ", entity.id);
            }
        }
    }

    void EntityManager::Clear() {
        m_Entities.clear();
        m_ComponentManager.Clear();
    }

    void EntityManager::AddComponent(const Entity &entity, const std::string &typeName) {
        m_ComponentManager.AddComponent(entity.id, typeName);
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
