#include "Application.hpp"

#include "Entities/Components/Graphics/Camera.hpp"
#include "Entities/Components/Graphics/DirectionalLight.hpp"
#include "Entities/Components/Graphics/ModelRenderer.hpp"
#include "Entities/Components/Graphics/PointLight.hpp"
#include "Entities/Components/Transform.hpp"
#include "Event/Event.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/Renderer.hpp"
#include "Gui/Gui.hpp"
#include "Input/InputManager.hpp"
#include "Time/Clock.hpp"
#include "Window.hpp"

namespace Pixf::Core {
    void Application::Run() {
        this->OnAwake();

        while (!m_Window.ShouldClose() && m_IsRunning) {
            Window::PollEvents();
            m_EventManager.ProcessEvents();

            this->OnUpdate(m_Clock.GetDeltaTime());

            if (const auto activeWorld = m_WorldManager.GetActiveWorld(); activeWorld.IsSuccess()) {
                activeWorld.Unwrap()->Update(m_Clock.GetDeltaTime());
            }

            this->OnLateUpdate(m_Clock.GetDeltaTime());

            if (const auto activeWorld = m_WorldManager.GetActiveWorld(); activeWorld.IsSuccess()) {
                activeWorld.Unwrap()->LateUpdate(m_Clock.GetDeltaTime());
            }

            Render();

            this->OnRender(m_Clock.GetDeltaTime());

            if (const auto activeWorld = m_WorldManager.GetActiveWorld(); activeWorld.IsSuccess()) {
                activeWorld.Unwrap()->Render(m_Clock.GetDeltaTime());
            }

            Gui::BeginRenderGui(m_Clock.GetDeltaTime());

            this->OnRenderGui(m_Clock.GetDeltaTime());

            Gui::EndRenderGui();

            Window::SwapBuffers();

            m_Clock.Tick();
        }

        this->OnShutdown();
        Gui::Terminate();
        Audio::AudioEngine::Terminate();
    }

    void Application::Exit() { m_IsRunning = false; }

    Window &Application::GetWindow() { return m_Window; }

    Input::InputManager &Application::GetInputManager() { return m_InputManager; }

    Assets::AssetManager &Application::GetAssetManager() { return m_AssetManager; }

    Graphics::Renderer &Application::GetRenderer() { return m_Renderer; }

    Event::EventManager &Application::GetEventManager() { return m_EventManager; }

    WorldManager &Application::GetWorldManager() { return m_WorldManager; }

    Window Application::CreateWindow(const WindowConfig &config, Event::EventManager &eventManager) {
        Window window = Window::Create(config).Unwrap();
        window.SetRenderTarget(eventManager);
        return window;
    }

    void Application::Render() {
        auto worldResult = m_WorldManager.GetActiveWorld();
        if (worldResult.IsError()) {
            return;
        }

        Entities::World &world = *worldResult.Unwrap();
        Entities::EntityManager &entityManager = world.GetEntityManager();

        auto camResult = entityManager.GetSingleton<Entities::Components::Graphics::Camera>();
        if (camResult.IsError()) {
            return;
        }

        auto cam = camResult.Unwrap();

        auto dirLights = entityManager.Query<Entities::Components::Graphics::DirectionalLight>().UnwrapOr({});
        std::vector<Entities::Components::Graphics::DirectionalLight> dirLightsVec;
        for (const auto &[_, comp]: dirLights) {
            dirLightsVec.push_back(*comp);
        }

        auto pointLights = entityManager.Query<Entities::Components::Graphics::PointLight>().UnwrapOr({});
        std::vector<Entities::Components::Graphics::PointLight> pointLightsVec;
        for (const auto &[_, comp]: pointLights) {
            pointLightsVec.push_back(*comp);
        }

        auto ambientLight = entityManager.GetSingleton<Entities::Components::Graphics::AmbientLight>().UnwrapOr({});

        for (auto query = world.GetEntityManager().Query<Entities::Components::Graphics::ModelRenderer>();
             const auto &[id, comp]: query.Unwrap()) {
            auto meshes = m_AssetManager.GetModel(comp->model).Unwrap()->GetMeshes();
            auto materials = m_AssetManager.GetModel(comp->model).Unwrap()->GetMaterials();

            const Entities::Entity entity = entityManager.GetEntity(id).value();
            Entities::Components::Transform transform;
            if (auto result = entityManager.GetComponent<Entities::Components::Transform>(entity); result.IsError()) {
                transform = {};
            } else {
                transform = *result.Unwrap();
            }

            for (size_t i = 0; i < meshes.size(); i++) {
                Graphics::RenderCommand cmd = {
                        .ambientLight = *ambientLight,
                        .directionalLights = dirLightsVec,
                        .pointLights = pointLightsVec,
                        .mesh = meshes[i],
                        .material = materials[i],
                        .model = transform.GetMatrix(),
                        .view = cam->GetViewMatrix(),
                        .projection = cam->GetProjectionMatrix(),
                };

                GetRenderer().GetRenderQueue().Push(cmd, Graphics::RenderType::Opaque);
            }
        }

        m_Renderer.Render(m_AssetManager);
    }

    Application::Application(const ApplicationConfig &config) :
        m_Window(CreateWindow(config.windowConfig, m_EventManager)), m_Renderer(config.rendererConfig),
        m_InputManager(m_EventManager, m_Window), m_WorldManager(*this), m_AppConfig(config) {
        PIXF_ASSERT(Audio::AudioEngine::Init(config.audioManagerConfig) == Audio::AudioEngineError::None,
                    "Failed to initialize audio engine!");
        Gui::Init(m_EventManager);
    }
} // namespace Pixf::Core
