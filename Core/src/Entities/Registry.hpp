#ifndef PIXF_REGISTRY_HPP
#define PIXF_REGISTRY_HPP

#include <entt/entt.hpp>

#include "Common.hpp"

namespace Pixf::Core::Entities {
    using Entity = entt::entity;

    class PIXF_API Registry {
    public:
        Entity CreateEntity();
        void DestroyEntity(Entity);

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
        T GetComponent(const Entity entity) {
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
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_REGISTRY_HPP
