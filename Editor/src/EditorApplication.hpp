#ifndef EDITORAPPLICATION_HPP
#define EDITORAPPLICATION_HPP

#include <optional>

#include "Audio/AudioEngine.hpp"
#include "Console.hpp"
#include "Entities/World.hpp"
#include "Graphics/Renderer.hpp"
#include "Time/Clock.hpp"
#include "Window.hpp"
#include "WorldManager.hpp"
#include "Math/Math.hpp"

namespace Pixf::Editor {
    struct EditorConfig {
        Core::WindowConfig windowConfig = {
                .title = "Editor", .size = Core::Math::Vector2u(1920, 1080), .samplesPerPixel = 4, .vsync = true};

        Core::Graphics::RendererConfig rendererConfig = {.viewportAspect = Core::Math::Vector2i(1920, 1080)};

        Core::Audio::AudioManagerConfig audioManagerConfig;
    };

    class EditorApplication final : public Core::Entities::Context {
    public:
        explicit EditorApplication(const EditorConfig &config);

        EditorApplication(const EditorApplication &other) = delete;
        EditorApplication(EditorApplication &&other) = delete;
        EditorApplication &operator=(const EditorApplication &) = delete;
        EditorApplication &operator=(EditorApplication &&) = delete;

        ~EditorApplication() override;

        void OnAwake();
        void OnUpdate(double deltaTime);
        void OnRender(double deltaTime);
        void OnRenderGui(double deltaTime);
        void OnShutdown();

        void RenderTopBar(Core::Math::Vector2i origin, Core::Math::Vector2i aspect);
        void RenderHierarchy(Core::Math::Vector2i origin, Core::Math::Vector2i aspect);
        void RenderInspector(Core::Math::Vector2i origin, Core::Math::Vector2i aspect);
        void RenderFileBrowser(Core::Math::Vector2i origin, Core::Math::Vector2i aspect);
        void RenderConsole(Core::Math::Vector2i origin, Core::Math::Vector2i aspect) const;

        void Run();
        void Exit();

        Core::Window &GetWindow() override;
        Core::Input::InputManager &GetInputManager() override;
        Core::Assets::AssetManager &GetAssetManager() override;
        Core::Graphics::Renderer &GetRenderer() override;
        Core::Event::EventManager &GetEventManager() override;
        Core::WorldManager &GetWorldManager() override;

        Console &GetConsole();

    private:
        Core::Event::EventManager m_EventManager;
        Core::Window m_Window;
        Core::Assets::AssetManager m_AssetManager;
        Core::Graphics::Renderer m_Renderer;
        Core::Input::InputManager m_InputManager;
        Core::WorldManager m_WorldManager;
        Core::Time::Clock m_Clock;
        EditorConfig m_AppConfig;
        bool m_IsRunning = true;

        Console m_Console;
        std::optional<Core::Entities::Entity> m_SelectedEntity;

        static Core::Window CreateWindow(const Core::WindowConfig &config, Core::Event::EventManager &eventManager);

        void Render();
    };
} // namespace Pixf::Editor

#endif // EDITORAPPLICATION_HPP
