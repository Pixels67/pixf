#ifndef SYSTEMSMANAGER_HPP
#define SYSTEMSMANAGER_HPP

#include <unordered_map>

namespace Pixf::Core::Entities {
    class World;

    struct System {
        virtual ~System() = default;

        virtual void OnAwake(World &entityManager) {}

        virtual void OnUpdate(World &entityManager, double deltaTime) {}
        virtual void OnLateUpdate(World &entityManager, double deltaTime) {}
        virtual void OnRender(World &entityManager) {}
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

        void OnAwake(World &world);
        void OnUpdate(World &world, double deltaTime);
        void OnLateUpdate(World &world, double deltaTime);
        void OnRender(World &world);

    private:
        std::unordered_map<unsigned int, std::shared_ptr<System>> m_Systems;
        unsigned int m_SystemCounter = 0;
    };
} // namespace Pixf::Core::Entities

#endif // SYSTEMSMANAGER_HPP
