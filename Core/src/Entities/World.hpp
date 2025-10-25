#ifndef WORLD_HPP
#define WORLD_HPP

#include "EntityManager.hpp"
#include "SystemsManager.hpp"
#include "Time/Clock.hpp"

namespace Pixf::Core::Entities {
    class World {
    public:
        World() = default;

        World(const World &other) = default;
        World(World &&other) = delete;
        World &operator=(const World &other) = default;
        World &operator=(World &&other) = delete;

        ~World() = default;

        EntityManager &GetEntityManager();

        void Awake();
        void Update(double deltaTime);
        void LateUpdate(double deltaTime);
        void Render();

    private:
        EntityManager m_EntityManager;
        SystemsManager m_SystemsManager;
    };
} // namespace Pixf::Core::Entities

#endif // WORLD_HPP
