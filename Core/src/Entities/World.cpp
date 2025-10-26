#include "World.hpp"

#include "Application.hpp"

namespace Pixf::Core::Entities {
    World::World(Application &application) : m_Application(application) {}

    EntityManager &World::GetEntityManager() { return m_EntityManager; }

    SystemsManager &World::GetSystemsManager() { return m_SystemsManager; }

    Graphics::Gl::Window &World::GetWindow() const { return m_Application.GetWindow(); }

    Input::InputManager &World::GetInputManager() const { return m_Application.GetInputManager(); }

    Graphics::Renderer &World::GetRenderer() const { return m_Application.GetRenderer(); }

    Audio::AudioManager &World::GetAudioManager() const { return m_Application.GetAudioManager(); }

    Event::EventManager &World::GetEventManager() const { return m_Application.GetEventManager(); }

    WorldManager &World::GetWorldManager() const { return m_Application.GetWorldManager(); }

    void World::Awake() { m_SystemsManager.OnAwake(*this); }

    void World::Update(const double deltaTime) { m_SystemsManager.OnUpdate(*this, deltaTime); }

    void World::LateUpdate(const double deltaTime) { m_SystemsManager.OnLateUpdate(*this, deltaTime); }

    void World::Render(const double deltaTime) { m_SystemsManager.OnRender(*this, deltaTime); }
} // namespace Pixf::Core::Entities
