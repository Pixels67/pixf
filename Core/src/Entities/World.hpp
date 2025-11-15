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

    class Window;
} // namespace Pixf::Core

namespace Pixf::Core::Entities {
    class PIXF_API Context {
    public:
        virtual ~Context() = default;

        virtual Window &GetWindow() = 0;
        virtual Input::InputManager &GetInputManager() = 0;
        virtual Graphics::Renderer &GetRenderer() = 0;
        virtual Assets::AssetManager &GetAssetManager() = 0;
        virtual Event::EventManager &GetEventManager() = 0;
        virtual WorldManager &GetWorldManager() = 0;
    };

    class PIXF_API World final : Serialization::Serializable {
    public:
        explicit World(Context &context, const Blueprint &blueprint);

        World(const World &other) = default;
        World(World &&other) = delete;
        World &operator=(const World &other) = delete;
        World &operator=(World &&other) = delete;

        ~World() override = default;

        EntityManager &GetEntityManager();
        SystemsManager &GetSystemsManager();

        Context &GetContext() const;

        void Awake();
        void Update(double deltaTime);
        void LateUpdate(double deltaTime);
        void Render(double deltaTime);

        Serialization::Json::object Serialize(bool editorMode = false) override { return m_EntityManager.Serialize(editorMode); }

        void Deserialize(const Serialization::Json::object &json, Assets::AssetManager &assetManager,
                         bool editorMode = false) override {
            m_EntityManager.Deserialize(json, assetManager, editorMode);
        }

    private:
        EntityManager m_EntityManager;
        SystemsManager m_SystemsManager;
        Context &m_Context;
    };
} // namespace Pixf::Core::Entities

#endif // WORLD_HPP
