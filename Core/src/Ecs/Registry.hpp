#ifndef FLK_REGISTRY_HPP
#define FLK_REGISTRY_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "Storage.hpp"
#include "TypeId.hpp"

namespace Flock::Ecs {
    struct EntityData {
        EntityVersion version = 0;
        bool          alive   = true;
    };

    /**
     * @class Registry
     * @brief ECS registry.
     */
    class FLK_API Registry {
    public:
        /**
         * Creates an entity and returns a handle to it.
         * @return A newly created entity.
         */
        Entity Create();

        /**
         * @brief Retrieves the entity with the corresponding ID.
         * @param id The entity's ID.
         * @return A handle to the entity if found; std::nullopt otherwise.
         */
        std::optional<Entity> Get(EntityId id) const;

        /**
         * @brief Destroys an entity if it's alive and valid.
         * @param entity A handle to the entity.
         * @return true if successful; false otherwise.
         */
        bool Destroy(Entity entity);

        /**
         * @brief Whether an entity is valid or not.
         * @param entity A handle to the entity.
         * @return true if it's valid; false otherwise.
         */
        bool IsValid(Entity entity) const;

        /**
         * @brief Whether an entity is alive or dead.
         * @param entity A handle to the entity.
         * @return true if it's alive; false otherwise.
         */
        bool IsAlive(Entity entity) const;

        /**
         * @brief Registers a component type to the registry.
         * @tparam T The component type.
         */
        template<typename T>
        void RegisterComponent() {
            m_Storages[GetTypeId<T>()] = std::make_unique<Storage<T> >();
        }

        /**
         * @brief Retrieves a reference to the storage for a component type.
         * @tparam T The component type.
         * @return A reference to the storage if successful; std::nullopt otherwise.
         */
        template<typename T>
        OptionalRef<Storage<T> > GetStorage() {
            if (!IsComponentRegistered<T>()) {
                return std::nullopt;
            }

            return *static_cast<Storage<T> *>(m_Storages.at(GetTypeId<T>()).get());
        }

        /**
         * @brief Whether a component type is registered or not.
         * @tparam T The component type.
         * @return true if the component type is registered; false otherwise.
         */
        template<typename T>
        bool IsComponentRegistered() const {
            return m_Storages.contains(GetTypeId<T>());
        }

        /**
         * @brief Whether an entity has a component or not.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @return true if the entity has the component; false otherwise.
         */
        template<typename T>
        bool HasComponent(Entity entity) {
            return IsComponentRegistered<T>() && GetStorage<T>().value().get().Has(entity.id);
        }

        /**
         * @brief Retrieves a reference to the component data of an entity.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @return A reference to the component data if it exists; std::nullopt otherwise.
         */
        template<typename T>
        OptionalRef<T> GetComponent(Entity entity) {
            if (!IsComponentRegistered<T>()) {
                Debug::LogErr("Failed to get component: {}!", "Component not registered");
                return std::nullopt;
            }

            if (!HasComponent<T>(entity)) {
                return std::nullopt;
            }

            return GetStorage<T>().value().get().Get(entity.id);
        }

        /**
         * @brief Adds a component to an entity, fails if the component already exists.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @param value The value to set for the data.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool AddComponent(Entity entity, T value = {}) {
            if (!IsComponentRegistered<T>()) {
                RegisterComponent<T>();
            }

            if (HasComponent<T>(entity)) {
                return false;
            }

            GetStorage<T>().value().get().Insert(entity.id, std::move(value));
            return true;
        }

        /**
         * @brief Removes a component to an entity, fails if the component does not exist.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool RemoveComponent(Entity entity) {
            if (!IsComponentRegistered<T>()) {
                RegisterComponent<T>();
            }

            if (!HasComponent<T>(entity)) {
                return false;
            }

            GetStorage<T>().value().get().Remove(entity.id);
            return true;
        }

        /**
         * @brief Clears all the components of an entity.
         * @param entity A handle to the entity.
         */
        void ClearComponents(Entity entity);

    private:
        std::vector<EntityData>                                m_EntityData;
        std::vector<EntityId>                                  m_DeadEntities;
        std::unordered_map<TypeId, std::unique_ptr<IStorage> > m_Storages;
    };
}

#endif //FLK_REGISTRY_HPP
