#include "World.hpp"

#include "Application.hpp"

namespace Pixf::Core::Entities {
    World::World(Context &context, const Blueprint &blueprint) : m_Context(context) {
        blueprint.Get()(m_EntityManager, m_SystemsManager);
    }

    EntityManager &World::GetEntityManager() { return m_EntityManager; }

    SystemsManager &World::GetSystemsManager() { return m_SystemsManager; }

    Context &World::GetContext() const { return m_Context; }

    void World::Awake() { m_SystemsManager.OnAwake(*this); }

    void World::Update(const double deltaTime) { m_SystemsManager.OnUpdate(*this, deltaTime); }

    void World::LateUpdate(const double deltaTime) { m_SystemsManager.OnLateUpdate(*this, deltaTime); }

    void World::Render(const double deltaTime) { m_SystemsManager.OnRender(*this, deltaTime); }
} // namespace Pixf::Core::Entities
