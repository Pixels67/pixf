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
    using namespace Graphics;
    using namespace Entities::Components;
    using namespace Entities::Components::Graphics;

    void Application::Run() {
        PIXF_LOG_INFO("Using OpenGL ", glGetString(GL_VERSION));
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

    Renderer &Application::GetRenderer() { return m_Renderer; }

    Event::EventManager &Application::GetEventManager() { return m_EventManager; }

    WorldManager &Application::GetWorldManager() { return m_WorldManager; }

    Window Application::CreateWindow(const WindowConfig &config, Event::EventManager &eventManager) {
        Window window = Window::Create(config).Unwrap("Failed to create window!");
        window.SetRenderTarget(eventManager);
        return window;
    }

    void Application::Render() {
        const auto worldResult = m_WorldManager.GetActiveWorld();
        if (worldResult.IsError()) {
            //PIXF_LOG_WARN("No active world!");
            return;
        }

        Entities::World &world = *worldResult.Unwrap();
        Entities::EntityManager &entityManager = world.GetEntityManager();

        const auto camResult = entityManager.GetSingleton<Camera>();
        if (camResult.IsError()) {
            PIXF_LOG_WARN("No camera found!");
            return;
        }

        const auto cam = camResult.Unwrap();

        std::vector<DirectionalLight> dirLights;
        entityManager.ForEach<DirectionalLight>([&dirLights](const DirectionalLight &light) {
            dirLights.push_back(light);
        });

        std::vector<PointLight> pointLights;
        entityManager.ForEach<PointLight>([&pointLights](const PointLight &light) {
            pointLights.push_back(light);
        });

        const auto ambientLight = entityManager.GetSingleton<AmbientLight>().UnwrapOr({});

        world.GetEntityManager().ForEachEntity<ModelRenderer>([&](Entities::Entity entity, ModelRenderer &comp) {
            auto meshes = m_AssetManager.GetModel(comp.model).Unwrap("Invalid ModelRenderer.model")->GetMeshes();
            auto materials = m_AssetManager.GetModel(comp.model).Unwrap("Invalid ModelRenderer.model")->GetMaterials();

            Transform transform;
            if (auto result = entityManager.GetComponent<Transform>(entity); result.IsError()) {
                transform = {};
            } else {
                transform = *result.Unwrap();
            }

            for (size_t i = 0; i < meshes.size(); i++) {
                RenderCommand cmd = {
                        .ambientLight = *ambientLight,
                        .directionalLights = dirLights,
                        .pointLights = pointLights,
                        .mesh = meshes[i],
                        .material = materials[i],
                        .model = transform.GetMatrix(),
                        .view = cam->GetViewMatrix(),
                        .projection = cam->GetProjectionMatrix(),
                };

                GetRenderer().GetRenderQueue().Push(cmd, RenderType::Opaque);
            }
        });

        m_Renderer.Render(m_AssetManager);
    }

    Application::Application(const ApplicationConfig &config) :
        m_Window(CreateWindow(config.windowConfig, m_EventManager)), m_Renderer(config.rendererConfig),
        m_InputManager(m_EventManager, m_Window), m_WorldManager(*this), m_AppConfig(config) {
        PIXF_ASSERT(Audio::AudioEngine::Init(config.audioManagerConfig) == Audio::AudioEngineError::None,
                    "Failed to initialize audio engine!");
        Gui::Init(m_EventManager);
    }

    Application::~Application() = default;

    void Application::OnAwake() {}

    void Application::OnUpdate(double deltaTime) {}

    void Application::OnLateUpdate(double deltaTime) {}

    void Application::OnRender(double deltaTime) {}

    void Application::OnRenderGui(double deltaTime) {}

    void Application::OnShutdown() {}
} // namespace Pixf::Core
