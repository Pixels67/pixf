#ifndef FLK_REGISTRY_HPP
#define FLK_REGISTRY_HPP

#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Common.hpp"
#include "Entity.hpp"
#include "Storage.hpp"
#include "TypeId.hpp"
#include "Debug/Log.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Ecs {
    class IStorage;
    template<typename T>
    class Storage;
}

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
        std::vector<EntityData>                                m_EntityData;
        std::vector<EntityId>                                  m_DeadEntities;
        std::unordered_map<TypeId, std::shared_ptr<IStorage> > m_Storages;

        std::unordered_map<TypeId, std::function<void(IStorage &, Serial::IArchive &)> > m_ArchiveFns;

    public:
        struct Collection {
            std::vector<Entity> entities = {};
            Registry *          registry = nullptr;

            template<typename... Args, typename F>
            void ForEach(F &&callback, bool includeDisabled = false) {
                for (Entity e: entities) {
                    if (registry->HasAll<Args...>(e) && registry->AllEnabled<Args...>(e) && !includeDisabled) {
                        callback(registry->Get<Args>(e).value().get()...);
                    }

                    if (registry->HasAll<Args...>(e) && includeDisabled) {
                        callback(registry->Get<Args>(e).value().get()...);
                    }
                }
            }
        };

        /**
         * @brief Clears the registry.
         */
        void Clear();

        /**
         * @brief Creates an entity and returns a handle to it.
         * @return A newly created entity.
         */
        Entity Create();

        /**
         * @brief Creates an entity; adds the specified components, and returns a handle to it.
         * @tparam Args The component types.
         * @param args The components.
         */
        template<typename... Args>
        Entity Create(Args... args) {
            Entity entity = Create();
            (AddComponent(entity, args), ...);

            return entity;
        }

        /**
         * @brief Retrieves the entity with the corresponding ID.
         * @param id The entity's ID.
         * @return A handle to the entity if found; std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<Entity> EntityWithId(EntityId id) const;

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
        [[nodiscard]] bool IsValid(Entity entity) const;

        /**
         * @brief Whether an entity is alive or dead.
         * @param entity A handle to the entity.
         * @return true if it's alive; false otherwise.
         */
        [[nodiscard]] bool IsAlive(Entity entity) const;

        /**
         * @brief Registers a component type to the registry.
         * @tparam T The component type.
         */
        template<typename T>
        void Register() {
            m_Storages.emplace(GetTypeId<T>(), std::make_shared<Ecs::Storage<T> >());

            if constexpr (Serial::Serializable<T>) {
                m_ArchiveFns[GetTypeId<T>()] = [](IStorage &storage, Serial::IArchive &archive) {
                    auto &store = static_cast<Ecs::Storage<T> &>(storage);
                    store.Archive(archive);
                };
            }
        }

        /**
         * @brief Retrieves a pointer to the storage for a component type.
         * @tparam T The component type.
         * @return A pointer to the storage if successful; nullptr otherwise.
         */
        template<typename T>
        Storage<T> *Storage() const {
            if constexpr (std::is_same_v<T, Entity>) {
                return nullptr;
            }

            if (!IsRegistered<T>()) {
                return nullptr;
            }

            return static_cast<Ecs::Storage<T> *>(m_Storages.at(GetTypeId<T>()).get());
        }

        /**
         * @brief Whether a component type is registered or not.
         * @tparam T The component type.
         * @return true if the component type is registered; false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool IsRegistered() const {
            if constexpr (std::is_same_v<T, Entity>) {
                return true;
            }

            return m_Storages.contains(GetTypeId<T>());
        }

        /**
         * @brief Whether an entity has a component or not.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @return true if the entity has the component; false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool Has(Entity entity) const {
            if constexpr (std::is_same_v<T, Entity>) {
                return true;
            }

            return IsRegistered<T>() && Storage<T>()->Has(entity.id);
        }

        /**
         * @brief Whether an entity has all the specified components or not.
         * @tparam Args The component types.
         * @param entity A handle to the entity.
         * @return true if the entity has all the components; false otherwise.
         */
        template<typename... Args>
        [[nodiscard]] bool HasAll(const Entity entity) const {
            return ((IsRegistered<Args>() && Has<Args>(entity)) && ...);
        }

        /**
         * @brief Whether an entity has any of the specified components or not.
         * @tparam Args The component types.
         * @param entity A handle to the entity.
         * @return true if the entity has any of the components; false otherwise.
         */
        template<typename... Args>
        [[nodiscard]] bool HasAny(const Entity entity) const {
            return ((IsRegistered<Args>() && Has<Args>(entity)) || ...);
        }

        /**
         * @brief Whether the component is enabled or not.
         * @param entity The entity.
         * @return true if the component is enabled; false otherwise.
         */
        template<typename T>
        bool IsEnabled(Entity entity) {
            if constexpr (std::is_same_v<T, Entity>) {
                return true;
            } else {
                return IsRegistered<T>() && Storage<T>()->IsEnabled(entity.id);
            }
        }

        /**
         * @brief Whether any one of the components is enabled or not.
         * @param entity The entity.
         * @return true if one of the components is enabled; false otherwise.
         */
        template<typename... Args>
        bool AnyEnabled(const Entity entity) {
            return (IsEnabled<Args>(entity) || ...);
        }

        /**
         * @brief Whether all the components are enabled or not.
         * @param entity The entity.
         * @return true if the components are enabled; false otherwise.
         */
        template<typename... Args>
        bool AllEnabled(const Entity entity) {
            return (IsEnabled<Args>(entity) && ...);
        }

        /**
         * @brief Sets the component enabled state.
         * @param entity The entity.
         * @param enabled The boolean value to set.
         * @return true successful; false otherwise.
         */
        template<typename T>
        bool SetEnabled(Entity entity, bool enabled = true) {
            return IsRegistered<T>() && Storage<T>()->SetEnabled(entity.id, enabled);
        }

        /**
         * @brief Sets all components' enabled state of a type.
         * @param enabled The boolean value to set.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool SetAllEnabled(bool enabled = true) {
            if (!IsRegistered<T>()) {
                return false;
            }

            Storage<T>()->SetAllEnabled(enabled);
            return true;
        }

        template<typename T>
        bool Enable(const Entity entity) {
            return SetEnabled<T>(entity);
        }

        template<typename T>
        bool Disable(const Entity entity) {
            return SetEnabled<T>(entity, false);
        }

        template<typename T>
        bool EnableAll() {
            return SetAllEnabled<T>();
        }

        template<typename T>
        bool DisableAll() {
            return SetAllEnabled<T>(false);
        }

        /**
         * @brief Retrieves a reference to the component data of an entity.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @return A reference to the component data if it exists; std::nullopt otherwise.
         */
        template<typename T>
            requires (!std::same_as<T, Entity>)
        T *Get(Entity entity) {
            if (!IsRegistered<T>()) {
                Debug::LogErr("Failed to get component: {}!", "Component not registered");
                return nullptr;
            }

            if (!Has<T>(entity)) {
                return nullptr;
            }

            return Storage<T>()->Get(entity.id);
        }

        template<typename T>
            requires std::same_as<T, Entity>
        const T *Get(const Entity &entity) {
            return &entity;
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
            if constexpr (std::is_same_v<T, Entity>) {
                return true;
            }

            if (!IsRegistered<T>()) {
                Register<T>();
            }

            if (Has<T>(entity)) {
                return false;
            }

            Storage<T>()->Insert(entity.id, std::move(value));
            return true;
        }

        /**
         * @brief Retrieves a reference to the component data of an entity and adds it if it doesn't exist.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @param value The value to set for the data.
         * @return A reference to the component.
         */
        template<typename T>
        T &GetOrAdd(Entity entity, T value = {}) {
            if (!IsRegistered<T>()) {
                Register<T>();
            }

            if (!Has<T>(entity)) {
                AddComponent(entity, value);
            }

            return *Get<T>(entity);
        }

        /**
         * @brief Changes an entity's component data, fails if the component doesn't exist.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @param value The value to set for the data.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool Set(Entity entity, T value = {}) {
            if constexpr (std::is_same_v<T, Entity>) {
                return false;
            }

            if (!IsRegistered<T>()) {
                Register<T>();
            }

            if (!Has<T>(entity)) {
                return false;
            }

            Storage<T>()->Insert(entity.id, std::move(value));
            return true;
        }

        /**
         * @brief Removes a component to an entity, fails if the component does not exist.
         * @tparam T The component type.
         * @param entity A handle to the entity.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool Remove(Entity entity) {
            if constexpr (std::is_same_v<T, Entity>) {
                return false;
            }

            if (!IsRegistered<T>()) {
                Register<T>();
            }

            if (!Has<T>(entity)) {
                return false;
            }

            Storage<T>()->Remove(entity.id);
            return true;
        }

        /**
         * @brief Clears all the components of an entity.
         * @param entity A handle to the entity.
         */
        void Clear(Entity entity);

        /**
         * @brief Invokes a callback for each entity with its components.
         * @tparam First The smallest storage.
         * @tparam Args The component types.
         * @tparam F The callback type.
         * @param callback The callback to execute.
         * @param includeDisabled Whether to include disabled components or not.
         */
        template<typename First, typename... Args, typename F>
        void ForEach(F &&callback, bool includeDisabled = false) {
            if (!m_Storages.contains(GetTypeId<First>())) {
                return;
            }

            auto &storage = m_Storages.at(GetTypeId<First>());
            for (const EntityId id: storage->Dense()) {
                auto maybeEntity = EntityWithId(id);
                if (!maybeEntity) {
                    continue;
                }

                Entity entity = maybeEntity.value();
                if (HasAll<Args...>(entity) && AllEnabled<Args...>(entity) && !includeDisabled) {
                    callback(*Get<First>(entity), *Get<Args>(entity)...);
                }

                if (HasAll<Args...>(entity) && includeDisabled) {
                    callback(*Get<First>(entity), *Get<Args>(entity)...);
                }
            }
        }

        /**
         * @brief Retrieves a collection containing all the entities with the component types for iteration.
         * @tparam First The smallest storage.
         * @tparam Args The component types.
         * @return A collection containing all the entities with the component types.
         */
        template<typename First, typename... Args>
        Collection All() {
            Collection collection = {.registry = this};

            if (!m_Storages.contains(GetTypeId<First>())) {
                return collection;
            }

            for (const EntityId id: m_Storages.at(GetTypeId<First>())->Dense()) {
                auto maybeEntity = EntityWithId(id);
                if (!maybeEntity) {
                    continue;
                }

                Entity e = maybeEntity.value();
                if (HasAll<Args...>(e)) {
                    collection.entities.push_back(e);
                }
            }

            return collection;
        }

        void Archive(Serial::IArchive &archive);
    };
}

#endif //FLK_REGISTRY_HPP
