#include "World.hpp"

namespace Pixf::Core::Entities {
    EntityManager &World::GetEntityManager() { return m_EntityManager; }

    void World::Awake() { m_SystemsManager.OnAwake(m_EntityManager); }

    void World::Update(const double deltaTime) { m_SystemsManager.OnUpdate(m_EntityManager, deltaTime); }

    void World::LateUpdate(const double deltaTime) { m_SystemsManager.OnLateUpdate(m_EntityManager, deltaTime); }

    void World::Render() { m_SystemsManager.OnRender(m_EntityManager); }
} // namespace Pixf::Core::Entities
