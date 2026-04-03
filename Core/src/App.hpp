#ifndef FLK_APP_HPP
#define FLK_APP_HPP

#include "Common.hpp"
#include "Asset/AssetLoader.hpp"
#include "Audio/AudioPlayer.hpp"
#include "Ecs/Schedule.hpp"
#include "Ecs/World.hpp"
#include "Event/EventHandler.hpp"
#include "Glfw/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Gui/GuiRenderer.hpp"
#include "Input/InputHandler.hpp"
#include "Physics/PhysicsEngine.hpp"

namespace Flock {
    /**
     * @struct Services
     */
    struct FLK_API Services {
        Event::EventHandler    eventHandler;
        Glfw::Window           window;
        Asset::AssetLoader     assetLoader;
        Graphics::Renderer     renderer;
        Gui::GuiRenderer       guiRenderer;
        Input::InputHandler    inputHandler;
        Audio::AudioPlayer     audioPlayer;
        Physics::PhysicsEngine physicsEngine;
    };

    struct FLK_API AppConfig {
        Glfw::WindowConfig     windowConfig;
        Graphics::ShadowConfig shadowConfig;
    };

    /**
     * @class App
     */
    class FLK_API App {
        Ecs::World    m_World;
        Ecs::Schedule m_Schedule;
        Services      m_Services;
        AppConfig     m_Config;
        bool          m_ShouldClose = false;

    public:
        /**
         * @brief Static factory method.
         * @param config The app config.
         * @return A newly created app.
         */
        static std::optional<App> Create(const AppConfig &config = {});

        App() = default;
        ~App();

        App(const App &other)     = delete;
        App(App &&other) noexcept = default;

        App &operator=(const App &other)     = delete;
        App &operator=(App &&other) noexcept = default;

        /**
         * @brief Adds a system to a stage.
         * @param stage The stage.
         * @param system The system.
         * @return A reference to the app.
         */
        App &AddSystem(Ecs::Stage stage, const Ecs::System &system);

        /**
         * @brief Pops the last added system from a stage.
         * @param stage The stage.
         * @return A reference to the app.
         */
        App &PopSystem(Ecs::Stage stage);

        /**
         * @brief Runs the application loop until the app is closed.
         * @return A reference to the app.
         */
        App &Run();

        [[nodiscard]] Services &GetServices();

    private:
        void Prepare();
        void Extract();

        void Render();
        void RenderGui();
    };

    struct FLK_API Application {
        bool shouldClose = false;

        void Close() {
            shouldClose = true;
        }
    };
}

#endif //FLK_APP_HPP
