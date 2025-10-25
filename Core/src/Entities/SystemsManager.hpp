#ifndef SYSTEMSMANAGER_HPP
#define SYSTEMSMANAGER_HPP

#include <unordered_map>

#include "EntityManager.hpp"

namespace Pixf::Core::Entities {
    struct System {
        virtual ~System() = default;

        virtual void OnAwake(EntityManager &entityManager) {}

        virtual void OnUpdate(EntityManager &entityManager, double deltaTime) {}
        virtual void OnLateUpdate(EntityManager &entityManager, double deltaTime) {}
        virtual void OnRender(EntityManager &entityManager) {}
    };

    class SystemsManager {
    public:
        SystemsManager() = default;

        SystemsManager(const SystemsManager &) = default;
        SystemsManager(SystemsManager &&) = delete;
        SystemsManager &operator=(const SystemsManager &) = default;
        SystemsManager &operator=(SystemsManager &&) = delete;

        ~SystemsManager() = default;

        template<typename T>
        unsigned int AddSystem() {
            static_assert(std::is_base_of_v<System, T>, "T must derive from System");

            m_SystemCounter++;
            m_Systems.emplace(m_SystemCounter, std::make_shared<T>());
            return m_SystemCounter;
        }

        void RemoveSystem(unsigned int systemId);

        void OnAwake(EntityManager &entityManager);
        void OnUpdate(EntityManager &entityManager, double deltaTime);
        void OnLateUpdate(EntityManager &entityManager, double deltaTime);
        void OnRender(EntityManager &entityManager);

    private:
        std::unordered_map<unsigned int, std::shared_ptr<System>> m_Systems;
        unsigned int m_SystemCounter = 0;
    };
} // namespace Pixf::Core::Entities

#endif // SYSTEMSMANAGER_HPP
