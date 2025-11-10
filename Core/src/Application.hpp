#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Audio/AudioManager.hpp"
#include "Event/Event.hpp"
#include "Graphics/Gl/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Input/InputManager.hpp"
#include "Time/Clock.hpp"
#include "WorldManager.hpp"

namespace Pixf::Core {
    struct ApplicationConfig {
        Graphics::Gl::WindowConfig windowConfig;
        Graphics::RendererConfig rendererConfig;
        Audio::AudioManagerConfig audioManagerConfig;
    };

    class Application {
    public:
        explicit Application(const ApplicationConfig &config);
        virtual ~Application() = default;

        virtual void OnAwake() {}
        virtual void OnUpdate(double deltaTime) {}
        virtual void OnLateUpdate(double deltaTime) {}
        virtual void OnRender(double deltaTime) {}
        virtual void OnRenderGui(double deltaTime) {}
        virtual void OnShutdown() {}

        void Run();
        void Exit();

        Graphics::Gl::Window &GetWindow();
        Input::InputManager &GetInputManager();
        Assets::AssetManager &GetAssetManager();
        Graphics::Renderer &GetRenderer();
        Audio::AudioManager &GetAudioManager();
        Event::EventManager &GetEventManager();
        WorldManager &GetWorldManager();

    private:
        Event::EventManager m_EventManager = {};
        Graphics::Gl::Window m_Window;
        Assets::AssetManager m_AssetManager;
        Graphics::Renderer m_Renderer;
        Audio::AudioManager m_AudioManager;
        Input::InputManager m_InputManager;
        WorldManager m_WorldManager;
        Time::Clock m_Clock;
        ApplicationConfig m_AppConfig;
        bool m_IsRunning = true;

        static Graphics::Gl::Window CreateWindow(const Graphics::Gl::WindowConfig &config,
                                                 Event::EventManager &eventManager);

        void Render();
    };
} // namespace Pixf::Core

#define PIXF_RUN_APPLICATION(x)                                                                                        \
    int main() {                                                                                                       \
        x app;                                                                                                         \
        app.Run();                                                                                                     \
    }

#endif // APPLICATION_HPP
