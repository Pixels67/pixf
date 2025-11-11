#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Audio/AudioEngine.hpp"
#include "Event/Event.hpp"
#include "Graphics/Renderer.hpp"
#include "Input/InputManager.hpp"
#include "Time/Clock.hpp"
#include "Window.hpp"
#include "WorldManager.hpp"

namespace Pixf::Core {
    struct ApplicationConfig {
        WindowConfig windowConfig;
        Graphics::RendererConfig rendererConfig;
        Audio::AudioManagerConfig audioManagerConfig;
    };

    class PIXF_API Application {
    public:
        explicit Application(const ApplicationConfig &config);
        virtual ~Application();

        virtual void OnAwake();
        virtual void OnUpdate(double deltaTime);
        virtual void OnLateUpdate(double deltaTime);
        virtual void OnRender(double deltaTime);
        virtual void OnRenderGui(double deltaTime);
        virtual void OnShutdown();

        void Run();
        void Exit();

        Window &GetWindow();
        Input::InputManager &GetInputManager();
        Assets::AssetManager &GetAssetManager();
        Graphics::Renderer &GetRenderer();
        Event::EventManager &GetEventManager();
        WorldManager &GetWorldManager();

    private:
        Event::EventManager m_EventManager = {};
        Window m_Window;
        Assets::AssetManager m_AssetManager;
        Graphics::Renderer m_Renderer;
        Input::InputManager m_InputManager;
        WorldManager m_WorldManager;
        Time::Clock m_Clock;
        ApplicationConfig m_AppConfig;
        bool m_IsRunning = true;

        static Window CreateWindow(const WindowConfig &config, Event::EventManager &eventManager);

        void Render();
    };
} // namespace Pixf::Core

#define PIXF_RUN_APPLICATION(x)                                                                                        \
    int main() {                                                                                                       \
        x app;                                                                                                         \
        app.Run();                                                                                                     \
    }

#endif // APPLICATION_HPP
