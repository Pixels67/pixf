#ifndef WORLD_HPP
#define WORLD_HPP

#include "Blueprint.hpp"
#include "EntityManager.hpp"
#include "SystemsManager.hpp"

namespace Pixf::Core {
    class WorldManager;
    namespace Event {
        class EventManager;
    }

    namespace Audio {
        class AudioEngine;
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
    class PIXF_API World final : Serialization::Serializable {
    public:
        explicit World(Application &application, const Blueprint &blueprint);

        World(const World &other) = default;
        World(World &&other) = delete;
        World &operator=(const World &other) = delete;
        World &operator=(World &&other) = delete;

        ~World() override = default;

        EntityManager &GetEntityManager();
        SystemsManager &GetSystemsManager();
        Window &GetWindow() const;
        Input::InputManager &GetInputManager() const;
        Graphics::Renderer &GetRenderer() const;
        Assets::AssetManager &GetAssetManager() const;
        Event::EventManager &GetEventManager() const;
        WorldManager &GetWorldManager() const;

        void Awake();
        void Update(double deltaTime);
        void LateUpdate(double deltaTime);
        void Render(double deltaTime);

        Json::object Serialize() override {
            return m_EntityManager.Serialize();
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            m_EntityManager.Deserialize(json, assetManager);
        }

    private:
        EntityManager m_EntityManager;
        SystemsManager m_SystemsManager;
        Application &m_Application;
    };
} // namespace Pixf::Core::Entities

#endif // WORLD_HPP
