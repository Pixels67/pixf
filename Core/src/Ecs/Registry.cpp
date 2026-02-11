#include "Registry.hpp"

namespace Flock::Ecs {
    Entity Registry::Create() {
        if (!m_DeadEntities.empty()) {
            const EntityId id = m_DeadEntities.back();
            m_DeadEntities.pop_back();

            m_EntityData[id].alive = true;
            m_EntityData[id].version++;

            return Entity{.id = id, .version = m_EntityData[id].version};
        }

        const EntityId entityId = m_EntityData.size();
        m_EntityData.push_back(EntityData{});

        return Entity{.id = entityId, .version = 0};
    }

    std::optional<Entity> Registry::Get(const EntityId id) const {
        if (id >= m_EntityData.size() || !m_EntityData.at(id).alive) {
            return std::nullopt;
        }

        return Entity{.id = id, .version = m_EntityData.at(id).version};
    }

    bool Registry::Destroy(const Entity entity) {
        if (entity.id >= m_EntityData.size()) {
            return false;
        }

        if (!IsAlive(entity)) {
            return false;
        }

        ClearComponents(entity);
        m_EntityData[entity.id].alive = false;
        m_DeadEntities.push_back(entity.id);

        return true;
    }

    bool Registry::IsValid(const Entity entity) const {
        return entity.id < m_EntityData.size();
    }

    bool Registry::IsAlive(const Entity entity) const {
        return IsValid(entity) &&
               m_EntityData.at(entity.id).alive &&
               m_EntityData.at(entity.id).version == entity.version;
    }

    void Registry::ClearComponents(const Entity entity) {
        if (!IsAlive(entity)) {
            return;
        }

        for (auto &[typeId, storage]: m_Storages) {
            storage->Remove(entity.id);
        }
    }
}
