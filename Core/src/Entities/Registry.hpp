#ifndef PIXF_REGISTRY_HPP
#define PIXF_REGISTRY_HPP

#include <entt/entt.hpp>

#include "Common.hpp"
#include "TypeId.hpp"

namespace Pixf::Core::Entities {
    using Entity = entt::entity;

    class PIXF_API Registry {
    public:
        Entity CreateEntity();
        void DestroyEntity(Entity);

        template<typename Archive, typename T>
        void Register(const std::string &name) {
            m_SerializeFns[GetTypeId<Archive>()][entt::type_hash<T>::value()] = [name](void *archivePtr, void *self) {
                auto &archive = *static_cast<Archive *>(archivePtr);
                auto *comp = static_cast<T *>(self);
                archive(name, *comp);
            };
        }

        template<typename Archive>
        void SerializeEntity(Archive &archive, Entity &entity) {
            archive.AddObject("components", [&](Archive &ar) { SerializeEntityComponents(ar, entity); });
            archive("id", entity);
        }

        template<typename Archive>
        void SerializeEntityComponents(Archive &archive, Entity &entity) {
            const TypeId archiveTypeId = GetTypeId<Archive>();
            if (!m_SerializeFns.contains(archiveTypeId)) {
                return;
            }

            for (auto [id, storage]: m_Registry.storage()) {
                if (!storage.contains(entity)) {
                    continue;
                }

                if (!m_SerializeFns[archiveTypeId].contains(id)) {
                    continue;
                }

                void *component = storage.value(entity);
                auto func = m_SerializeFns[archiveTypeId][id];
                func(&archive, component);
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

        template<typename... Args, typename Func>
        void ForEachEntity(Func func) {
            m_Registry.view<Args...>().each(func);
        }

    private:
        entt::registry m_Registry;
        std::unordered_map<TypeId, std::unordered_map<entt::id_type, std::function<void(void *, void *)>>>
                m_SerializeFns;
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_REGISTRY_HPP
