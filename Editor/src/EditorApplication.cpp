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
    using namespace Core::Entities;
    using namespace Core::Entities::Components;
    using namespace Core::Entities::Components::Graphics;
    using namespace Core::Entities::Components::Audio;

    constexpr float moveSpeed = 20.0F;
    constexpr float rotateSpeed = 60.0F;

    struct WorldNavigation final : System {
        void OnAwake(World &world) override {
            world.GetContext().GetEventManager().Subscribe<WindowSizeChangedEvent>(
                    [&](const WindowSizeChangedEvent &event) {
                        world.GetEntityManager().GetSingleton<Camera>().UnwrapOr({})->aspect =
                                static_cast<float>(event.newWidth) / static_cast<float>(event.newHeight);
                    });
        }

        void OnUpdate(World &world, const double deltaTime) override {
            EntityManager &entityManager = world.GetEntityManager();

            auto &cam = *entityManager.GetSingleton<Camera>().Unwrap();

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::W)) {
                cam.transform.Translate(cam.transform.rotation * vec3(0.0F, 0.0F, moveSpeed * deltaTime));
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::S)) {
                cam.transform.Translate(cam.transform.rotation * vec3(0.0F, 0.0F, -moveSpeed * deltaTime));
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::D)) {
                cam.transform.Translate(cam.transform.rotation * vec3(moveSpeed * deltaTime, 0.0F, 0.0F));
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::A)) {
                cam.transform.Translate(cam.transform.rotation * vec3(-moveSpeed * deltaTime, 0.0F, 0.0F));
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::E)) {
                cam.transform.Translate(cam.transform.rotation * vec3(0.0F, moveSpeed * deltaTime, 0.0F));
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::Q)) {
                cam.transform.Translate(cam.transform.rotation * vec3(0.0F, -moveSpeed * deltaTime, 0.0F));
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::Right)) {
                cam.transform.RotateAround(vec3(0.0F, 1.0F, 0.0F), rotateSpeed * deltaTime);
            }

            if (world.GetContext().GetInputManager().IsKeyDown(Input::Key::Left)) {
                cam.transform.RotateAround(vec3(0.0F, 1.0F, 0.0F), -rotateSpeed * deltaTime);
            }
        }
    };

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

        Blueprint blueprint;
        blueprint.Configure([&](EntityManager &entityManager, Entities::SystemsManager &systemsManager) {
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

            systemsManager.AddSystem<WorldNavigation>();
        });

        GetWorldManager().LoadWorld("world.json", "world", blueprint);
        GetWorldManager().SetActiveWorld("world");

        GetConsole().Log("Test info");
        GetConsole().LogWarning("Test warning");
        GetConsole().LogError("Test error");

        m_EventManager.Subscribe<Input::KeyEvent>([&](const Input::KeyEvent &event) {
            if (event.action != Input::KeyAction::Press)
                return;
            if (event.key != Input::Key::Delete)
                return;

            const auto result = GetWorldManager().GetActiveWorld();
            if (result.IsError())
                return;
            if (!m_SelectedEntity.has_value())
                return;

            result.Unwrap()->GetEntityManager().DestroyEntity(m_SelectedEntity.value());
        });
    }

    void EditorApplication::OnUpdate(double deltaTime) {}

    void EditorApplication::OnRender(double deltaTime) {}

    void EditorApplication::OnRenderGui(double deltaTime) {
        const ivec2 windowSize = GetWindow().GetSize();

        RenderTopBar(ivec2(0, 0), ivec2(windowSize.x, 50));
        RenderHierarchy(ivec2(0, 50), ivec2(300, 450));
        RenderInspector(ivec2(windowSize.x - 400, 50), ivec2(400, windowSize.y - 50));
        RenderFileBrowser(ivec2(0, 500), ivec2(300, windowSize.y - 500));
        RenderConsole(ivec2(300, windowSize.y - 300), ivec2(windowSize.x - 700, 300));
    }

    void EditorApplication::OnShutdown() {}

    void EditorApplication::RenderTopBar(const ivec2 origin, const ivec2 aspect) {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("##", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoTitleBar);

        Gui::SameLine(755);
        if (Gui::Button("Save", {200, 33})) {
            if (const auto result = GetWorldManager().GetActiveWorldName(); result.IsSuccess()) {
                const std::string &name = result.Unwrap();
                GetWorldManager().SaveWorld(name + ".json", name);
                GetConsole().Log("Saving world " + name + " To: " + name + ".json");
            } else {
                GetConsole().LogError("Failed to save world: No active world!");
            }
        }

        Gui::SameLine(965);
        if (Gui::Button("Run", {200, 33})) {
            // TODO: Running the runtime with the game
        }

        Gui::End();
    }

    void EditorApplication::RenderHierarchy(const ivec2 origin, const ivec2 aspect) {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("Hierarchy", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_MenuBar);

        auto &entityManager = GetWorldManager().GetActiveWorld().Unwrap("No active world!")->GetEntityManager();

        if (Gui::BeginMenuBar()) {
            if (Gui::BeginMenu("Entities")) {

                if (Gui::MenuItem("New")) {
                    const Entity e = entityManager.CreateEntity();
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
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        auto &entityManager = GetWorldManager().GetActiveWorld().Unwrap("No active world!")->GetEntityManager();

        if (!m_SelectedEntity.has_value()) {
            Gui::End();
            return;
        }

        Gui::Text("%s", m_SelectedEntity.value().GetName().c_str());

        entityManager.DeserializeEntityComponents(
                Gui::DrawJsonValue(Json::value_from(entityManager.SerializeEntityComponents(m_SelectedEntity.value())))
                        .as_object(),
                GetAssetManager(), m_SelectedEntity.value());

        Gui::End();
    }

    void EditorApplication::RenderFileBrowser(const ivec2 origin, const ivec2 aspect) {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("Files", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        Gui::DrawDirectoryRecursive("./Assets");

        Gui::End();
    }

    void EditorApplication::RenderConsole(const ivec2 origin, const ivec2 aspect) const {
        Gui::SetNextWindowPos({static_cast<float>(origin.x), static_cast<float>(origin.y)});
        Gui::SetNextWindowSize({static_cast<float>(aspect.x), static_cast<float>(aspect.y)});

        Gui::Begin("Console", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        m_Console.RenderAsGui();

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

    Console &EditorApplication::GetConsole() { return m_Console; }

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

        World &world = *worldResult.Unwrap();
        EntityManager &entityManager = world.GetEntityManager();

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
