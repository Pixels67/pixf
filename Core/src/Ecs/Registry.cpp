#include "Registry.hpp"

#include <string_view>

#include "Ecs/Entity.hpp"
#include "Ecs/Storage.hpp"

namespace Flock::Ecs {
    void Registry::Clear() {
        for (auto &[_, storage]: m_Storages) {
            storage->Clear();
        }

        m_EntityData.clear();
        m_DeadEntities.clear();
    }

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

    std::optional<Entity> Registry::EntityWithId(const EntityId id) const {
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

        Clear(entity);
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

    void Registry::Clear(const Entity entity) {
        if (!IsAlive(entity)) {
            return;
        }

        for (auto &[typeId, storage]: m_Storages) {
            storage->Remove(entity.id);
        }
    }

    void Registry::Archive(Serial::IArchive &archive) {
        usize size = m_EntityData.size();
        archive.BeginArray("entities", size);

        m_EntityData.resize(size);
        m_DeadEntities.clear();

        for (usize i = 0; i < size; i++) {
            archive.BeginObject();
            EntityId id = i;

            archive("id", id);
            archive("alive", m_EntityData[i].alive);

            if (!m_EntityData[i].alive) {
                m_DeadEntities.push_back(i);
            }

            u32 version = m_EntityData[i].version;
            archive("version", version);
            m_EntityData[i].version = version;

            if (id != i) {
                std::swap(m_EntityData[id], m_EntityData[i]);
            }

            archive.EndObject();
        }

        archive.EndArray();

        archive.BeginObject("components");

        for (auto &[typeId, fn]: m_ArchiveFns) {
            fn(*m_Storages[typeId], archive);
        }

        archive.EndObject();
    }
}
