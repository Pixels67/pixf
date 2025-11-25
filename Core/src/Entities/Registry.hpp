#ifndef PIXF_REGISTRY_HPP
#define PIXF_REGISTRY_HPP

#include <entt/entt.hpp>

#include "Common.hpp"
#include "Debug/Logger.hpp"
#include "TypeId.hpp"

namespace Pixf::Core::Entities {
    using Entity = entt::entity;

    class PIXF_API Registry {
    public:
        Entity CreateEntity();
        void DestroyEntity(Entity);

        template<typename Archive, typename T>
        void Register(const std::string &name) {
            m_SerializeFns[GetTypeId<Archive>()][entt::type_hash<T>::value()] = [name](void *archivePtr,
                                                                                       void *self) -> bool {
                auto &archive = *static_cast<Archive *>(archivePtr);
                auto *comp = static_cast<T *>(self);
                return archive(name, *comp);
            };

            // Initialize the storage
            m_Registry.storage<T>();
        }

        template<typename Archive>
        void SerializeEntity(Archive &archive, Entity &entity) {
            std::uint32_t uint = static_cast<std::uint32_t>(entity);
            archive("id", uint);
            // This is bad, but it hopefully shouldn't matter too much
            while (!m_Registry.valid(static_cast<entt::basic_registry<>::entity_type>(uint))) {
                m_Registry.create();
            }

            entity = static_cast<Entity>(uint);
            archive.Recurse("components", [&](Archive &ar) { SerializeEntityComponents(ar, entity); });
        }

        template<typename Archive>
        void SerializeEntityComponents(Archive &archive, Entity &entity) {
            const TypeId archiveTypeId = GetTypeId<Archive>();
            if (!m_SerializeFns.contains(archiveTypeId)) {
                return;
            }

            for (const auto &[id, storage]: m_Registry.storage()) {
                // If the component is not registered, skip to the next one
                if (!m_SerializeFns[archiveTypeId].contains(id)) {
                    continue;
                }

                // If we're serializing and the entity doesn't have the component, skip to the next one
                if (!storage.contains(entity) && archive.IsOutput()) {
                    continue;
                }

                // If we're deserializing and the entity doesn't have the component, add it
                if (!storage.contains(entity) && archive.IsInput()) {
                    storage.push(entity);
                }

                // If the archive function fails (the component key wasn't found in the archive) then remove
                // the component that was added earlier
                if (!m_SerializeFns[archiveTypeId][id](&archive, storage.value(entity))) {
                    storage.erase(entity);
                }
            }
        }

        template<typename T>
        void AddComponent(const Entity entity, const T component = {}) {
            if (m_Registry.any_of<T>(entity)) {
                m_Registry.remove<T>(entity);
            }

            m_Registry.emplace<T>(entity, component);
        }

        template<typename T>
        bool HasComponent(const Entity entity) const {
            return m_Registry.all_of<T>(entity);
        }

        template<typename... Args>
        bool HasComponents(const Entity entity) const {
            return m_Registry.all_of<Args...>(entity);
        }

        template<typename... Args>
        bool HasAnyComponents(const Entity entity) const {
            return m_Registry.any_of<Args...>(entity);
        }

        template<typename T>
        T &GetComponent(const Entity entity) {
            return m_Registry.get<T>(entity);
        }

        template<typename T>
        void RemoveComponent(const Entity entity) {
            m_Registry.remove<T>(entity);
        }

        template<typename... Args, typename Func>
        void ForEach(Func func) {
            m_Registry.view<Args...>().each(func);
        }

        template<typename Func>
        void ForEachEntity(Func func) {
            m_Registry.view<Entity>().each(func);
        }

    private:
        entt::registry m_Registry;
        std::map<TypeId, std::map<entt::id_type, std::function<bool(void *, void *)>>> m_SerializeFns;
    };

    template<typename Archive>
    void Serialize(Archive &archive, Registry &registry) {
        std::vector<Entity> entities;
        registry.ForEachEntity([&](const Entity entity) { entities.push_back(entity); });

        archive.template AddArray<Entity>("entities", entities, [&](Archive &ar, Entity &entity) {
            registry.SerializeEntity(ar, entity);
        });
    }
} // namespace Pixf::Core::Entities

#define PIXF_REGISTER_COMP(Component, Registry)                                                                        \
    Registry.Register<Pixf::Core::Serial::JsonOutputArchive, Component>(#Component);                                   \
    Registry.Register<Pixf::Core::Serial::JsonInputArchive, Component>(#Component)

#endif // PIXF_REGISTRY_HPP
