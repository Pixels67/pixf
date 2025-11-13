#include "EditorApplication.hpp"

#include "Entities/Components/Audio/AudioSource.hpp"
#include "Entities/Components/Graphics/Camera.hpp"
#include "Entities/Components/Graphics/ModelRenderer.hpp"
#include "Entities/Components/Transform.hpp"
#include "Graphics/Model.hpp"
#include "Gui/Gui.hpp"

namespace Pixf::Editor {
    using namespace Core;
    using namespace Core::Graphics;
    using namespace Core::Audio;
    using namespace Core::Entities::Components;
    using namespace Core::Entities::Components::Graphics;
    using namespace Core::Entities::Components::Audio;

    EditorApplication::EditorApplication(const EditorConfig &config) :
        m_Window(CreateWindow(config.windowConfig, m_EventManager)), m_Renderer(config.rendererConfig),
        m_InputManager(m_EventManager, m_Window), m_WorldManager(*this), m_AppConfig(config) {
        PIXF_ASSERT(AudioEngine::Init(config.audioManagerConfig) == AudioEngineError::None,
                    "Failed to initialize audio engine!");
        Gui::Init(m_EventManager);
    }

    EditorApplication::~EditorApplication() = default;

    void EditorApplication::OnAwake() {
        Debug::Logger::Init({.logLevel = Debug::Severity::Debug});

        Entities::Blueprint blueprint;
        blueprint.Configure([&](Entities::EntityManager &entityManager, Entities::SystemsManager &systemsManager) {
            entityManager.RegisterComponent<Transform>();
            entityManager.RegisterComponent<ModelRenderer>();
            entityManager.RegisterComponent<PointLight>();
            entityManager.RegisterComponent<AudioSource>();

            Camera camera{};
            camera.aspect = static_cast<float>(GetWindow().GetSize().x) / static_cast<float>(GetWindow().GetSize().y);
            camera.fov = 60.0F;
            camera.type = CameraType::Perspective;

            entityManager.CreateSingleton<Camera>(camera);
            entityManager.CreateSingleton<AmbientLight>();
        });

        //const Entities::World world(*this, blueprint);

        GetWorldManager().LoadWorld("world.json", "Unsaved", blueprint);
        GetWorldManager().SetActiveWorld("Unsaved");
    }

    void EditorApplication::OnUpdate(double deltaTime) {}

    void EditorApplication::OnRender(double deltaTime) {}

    void EditorApplication::OnRenderGui(double deltaTime) {
        const ivec2 windowSize = GetWindow().GetSize();

        RenderHierarchy(ivec2(0, 0), ivec2(300, 500));
        RenderInspector(ivec2(windowSize.x - 300, 0), ivec2(300, windowSize.y));
        RenderFileBrowser(ivec2(0, 500), ivec2(300, windowSize.y - 500));
    }

    void EditorApplication::OnShutdown() {}

    void EditorApplication::RenderHierarchy(const ivec2 origin, const ivec2 aspect) {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("Hierarchy", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar |
                           ImGuiWindowFlags_AlwaysVerticalScrollbar);

        auto &entityManager = GetWorldManager().GetActiveWorld().Unwrap("No active world!")->GetEntityManager();

        if (Gui::BeginMenuBar()) {
            if (Gui::BeginMenu("Entities")) {

                if (Gui::MenuItem("New")) {
                    const Entities::Entity e = entityManager.CreateEntity();
                    entityManager.AddComponent(e, Transform{});
                }

                Gui::EndMenu();
            }
            Gui::EndMenuBar();
        }

        for (const auto &entity: entityManager.GetAllEntities()) {
            std::string label = entity.GetName() + "##" + std::to_string(entity.GetId());
            if (Gui::Selectable(label.c_str())) {
                m_SelectedEntity = entity;
            }
        }

        Gui::End();
    }

    void EditorApplication::RenderInspector(const ivec2 origin, const ivec2 aspect) {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("Inspector", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar |
                           ImGuiWindowFlags_AlwaysVerticalScrollbar);

        auto &entityManager = GetWorldManager().GetActiveWorld().Unwrap("No active world!")->GetEntityManager();

        if (!m_SelectedEntity.has_value()) {
            Gui::End();
            return;
        }

        Gui::Text(m_SelectedEntity.value().GetName().c_str());

        if (const auto transform = entityManager.GetComponent<Transform>(m_SelectedEntity.value());
            transform.IsSuccess()) {
            transform.Unwrap()->Deserialize(
                    Gui::DrawJsonValue(Json::value_from(transform.Unwrap()->Serialize()), "Transform").as_object(),
                    GetAssetManager());
        }

        Gui::End();
    }

    void EditorApplication::RenderFileBrowser(const ivec2 origin, const ivec2 aspect) {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("Files", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar |
                           ImGuiWindowFlags_AlwaysVerticalScrollbar);

        Gui::DrawDirectoryRecursive(".", std::filesystem::path());

        Gui::End();
    }

    void EditorApplication::Run() {
        OnAwake();

        while (!m_Window.ShouldClose() && m_IsRunning) {
            Window::PollEvents();
            m_EventManager.ProcessEvents();

            OnUpdate(m_Clock.GetDeltaTime());

            if (const auto activeWorld = m_WorldManager.GetActiveWorld(); activeWorld.IsSuccess()) {
                activeWorld.Unwrap()->Update(m_Clock.GetDeltaTime());
            }

            if (const auto activeWorld = m_WorldManager.GetActiveWorld(); activeWorld.IsSuccess()) {
                activeWorld.Unwrap()->LateUpdate(m_Clock.GetDeltaTime());
            }

            Render();

            OnRender(m_Clock.GetDeltaTime());

            if (const auto activeWorld = m_WorldManager.GetActiveWorld(); activeWorld.IsSuccess()) {
                activeWorld.Unwrap()->Render(m_Clock.GetDeltaTime());
            }

            Gui::BeginRenderGui(m_Clock.GetDeltaTime());

            OnRenderGui(m_Clock.GetDeltaTime());

            Gui::EndRenderGui();

            Window::SwapBuffers();

            m_Clock.Tick();
        }

        OnShutdown();

        Gui::Terminate();
        m_AssetManager.DeleteAllAudioClips();
        AudioEngine::Terminate();
    }

    void EditorApplication::Exit() { m_IsRunning = false; }

    Window &EditorApplication::GetWindow() { return m_Window; }

    Input::InputManager &EditorApplication::GetInputManager() { return m_InputManager; }

    Assets::AssetManager &EditorApplication::GetAssetManager() { return m_AssetManager; }

    Renderer &EditorApplication::GetRenderer() { return m_Renderer; }

    Event::EventManager &EditorApplication::GetEventManager() { return m_EventManager; }

    WorldManager &EditorApplication::GetWorldManager() { return m_WorldManager; }

    Window EditorApplication::CreateWindow(const WindowConfig &config, Event::EventManager &eventManager) {
        Window window = Window::Create(config).Unwrap("Failed to create window!");
        window.SetRenderTarget(eventManager);
        return window;
    }

    void EditorApplication::Render() {
        m_Renderer.ClearViewport();

        const auto worldResult = m_WorldManager.GetActiveWorld();
        if (worldResult.IsError()) {
            return;
        }

        Entities::World &world = *worldResult.Unwrap();
        Entities::EntityManager &entityManager = world.GetEntityManager();

        const auto camResult = entityManager.GetSingleton<Camera>();
        if (camResult.IsError()) {
            return;
        }

        const auto cam = camResult.Unwrap();

        std::vector<DirectionalLight> dirLights;
        entityManager.ForEach<DirectionalLight>(
                [&dirLights](const DirectionalLight &light) { dirLights.push_back(light); });

        std::vector<PointLight> pointLights;
        entityManager.ForEach<PointLight>([&pointLights](const PointLight &light) { pointLights.push_back(light); });

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
} // namespace Pixf::Editor
