#include "Registry.hpp"

#include <entt/entt.hpp>

namespace Pixf::Core::Entities {
    uint32_t GetEntityId(const Entity entity) {
        return entt::to_integral(entity);
    }

    uint32_t GetEntityVersion(const Entity entity) {
        return entt::to_version(entity);
    }

    Entity Registry::CreateEntity(const std::string &name) {
        const Entity entity = m_Registry.create();
        SetEntityName(entity, name);
        return entity;
    }

    void Registry::DestroyEntity(const Entity entity) { m_Registry.destroy(entity); }

    std::string Registry::GetEntityName(const Entity entity) {
        if (m_EntityNames.size() > GetEntityId(entity)) {
            return m_EntityNames.at(GetEntityId(entity));
        }

        return "NULL";
    }

    std::vector<std::pair<Entity, std::string>> Registry::GetEntityNames() {
        std::vector<std::pair<Entity, std::string>> names{};
        ForEachEntity([&](const Entity entity) {
            names.push_back(std::make_pair(entity, GetEntityName(entity)));
        });

        return names;
    }

    void Registry::SetEntityName(const Entity entity, const std::string &name) {
        if (m_EntityNames.size() <= GetEntityId(entity)) {
            m_EntityNames.resize(GetEntityId(entity) + 1, "Entity");
        }

        m_EntityNames.at(GetEntityId(entity)) = name;
    }

    void Registry::SetEntityName(const uint32_t entityId, const std::string &name) {
        if (m_EntityNames.size() <= entityId) {
            m_EntityNames.resize(entityId + 1, "Entity");
        }

        m_EntityNames.at(entityId) = name;
    }
} // namespace Pixf::Core::Entities
