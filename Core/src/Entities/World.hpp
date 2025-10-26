#ifndef WORLD_HPP
#define WORLD_HPP

#include "EntityManager.hpp"
#include "SystemsManager.hpp"

namespace Pixf::Core {
    class WorldManager;
    namespace Event {
        class EventManager;
    }

    namespace Audio {
        class AudioManager;
    }

    namespace Graphics {
        class Renderer;
    }

    namespace Input {
        struct InputManager;
    }

    namespace Graphics::Gl {
        class Window;
    }

    class Application;
} // namespace Pixf::Core

namespace Pixf::Core::Entities {
    class World {
    public:
        explicit World(Application &application);

        World(const World &other) = default;
        World(World &&other) = delete;
        World &operator=(const World &other) = default;
        World &operator=(World &&other) = delete;

        ~World() = default;

        EntityManager &GetEntityManager();
        SystemsManager &GetSystemsManager();
        Graphics::Gl::Window &GetWindow() const;
        Input::InputManager &GetInputManager() const;
        Graphics::Renderer &GetRenderer() const;
        Audio::AudioManager &GetAudioManager() const;
        Event::EventManager &GetEventManager() const;
        WorldManager &GetWorldManager() const;

        void Awake();
        void Update(double deltaTime);
        void LateUpdate(double deltaTime);
        void Render();

    private:
        EntityManager m_EntityManager;
        SystemsManager m_SystemsManager;
        Application &m_Application;
    };
} // namespace Pixf::Core::Entities

#endif // WORLD_HPP
