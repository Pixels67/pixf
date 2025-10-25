#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <memory>
#include <optional>
#include <vector>
#include <unordered_map>

#include "ComponentManager.hpp"

namespace Pixf::Core::Entities {
    struct Entity {
        friend class EntityManager;
        explicit Entity() = default;
        explicit Entity(const unsigned int id) : id(id), active(true) {}

        unsigned int GetId() const { return id; }
        uint8_t GetVersion() const { return version; }
        bool IsActive() const { return active; }

    private:
        unsigned int id = 0;
        uint8_t version = 0;
        bool active = false;
    };

    enum class SingletonError : uint8_t {
        None = 0,
        AlreadyExists,
        NotFound,
    };

    class EntityManager {
    public:
        EntityManager() = default;

        EntityManager(const EntityManager &) = default;
        EntityManager(EntityManager &&) = delete;
        EntityManager &operator=(const EntityManager &) = default;
        EntityManager &operator=(EntityManager &&) = delete;

        ~EntityManager() = default;

        Entity CreateEntity();
        std::optional<Entity> GetEntity(unsigned int id) const;
        void DestroyEntity(Entity entity);
        void Clear();

        template<typename T>
        void RegisterComponent() {
            m_ComponentManager.RegisterComponent<T>();
        }

        template<typename T>
        void AddComponent(const Entity entity, const T component = T()) {
            m_ComponentManager.AddComponent<T>(entity.id, component);
        }

        template<typename T>
        Entity CreateEntityWithComponent(const T component = T()) {
            const Entity entity = CreateEntity();
            AddComponent<T>(entity, component);
            return entity;
        }

        template<typename T>
        bool HasComponent(const Entity entity) {
            return m_ComponentManager.HasComponent<T>(entity.id);
        }

        template<typename T>
        Error::Result<std::shared_ptr<T>, ComponentError> GetComponent(const Entity entity) {
            return m_ComponentManager.GetComponent<T>(entity.id);
        }

        template<typename T>
        void RemoveComponent(const Entity entity) {
            m_ComponentManager.RemoveComponent<T>(entity.id);
        }

        void ClearComponents(const Entity entity) const { m_ComponentManager.ClearComponents(entity.id); }

        template<typename T>
        Error::Result<std::unordered_map<unsigned int, std::shared_ptr<T>>, ComponentError> Query() {
            auto query = m_ComponentManager.QueryComponents<T>();
            if (query.IsError()) {
                return query.UnwrapError();
            }

            std::unordered_map<unsigned int, std::shared_ptr<T>> result;
            for (const auto &[id, comp]: query.Unwrap()->GetAll()) {
                result[id] = comp;
            }

            return result;
        }

        template<typename T>
        SingletonError CreateSingleton(T singleton = T()) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (m_Singletons.contains(std::type_index(typeid(T)))) {
                return SingletonError::AlreadyExists;
            }

            m_Singletons[std::type_index(typeid(T))] = std::make_shared<T>(singleton);
            return SingletonError::None;
        }

        template<typename T>
        Error::Result<std::shared_ptr<T>, SingletonError> GetSingleton() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (!m_Singletons.contains(std::type_index(typeid(T)))) {
                return SingletonError::NotFound;
            }

            return std::dynamic_pointer_cast<T>(m_Singletons[std::type_index(typeid(T))]);
        }

        template<typename T>
        void DestroySingleton() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (!m_Singletons.contains(std::type_index(typeid(T)))) {
                return;
            }

            m_Singletons.erase(std::type_index(typeid(T)));
        }

    private:
        std::vector<Entity> m_Entities;
        std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Singletons;
        ComponentManager m_ComponentManager;
        unsigned int m_EntityIdCounter = 0;

        size_t GenEntityId();
        std::optional<Entity *> GetFirstInactiveEntity();
    };

} // namespace Pixf::Core::Entities

#endif // ENTITYMANAGER_HPP
