#include "Editor.hpp"
#include "Entities/Components/Graphics/Camera.hpp"
#include "Input/InputManager.hpp"
#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Math;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Audio;
using namespace Pixf::Core::Debug;
using namespace Pixf::Core::Entities;
using namespace Pixf::Core::Entities::Components;
using namespace Pixf::Core::Entities::Components::Graphics;

namespace Pixf::Editor {
    struct InputSystem final : System {
        void Update(Registry &registry, const double deltaTime) override {
            if (Input::InputManager::IsKeyDown(Input::Key::W)) {
                auto &trans = registry.GetSingleton<Camera>().transform;
                trans.Translate(Vector3f::Forward() * 10.0F * deltaTime * trans.rotation);
            }

            if (Input::InputManager::IsKeyDown(Input::Key::S)) {
                auto &trans = registry.GetSingleton<Camera>().transform;
                trans.Translate(Vector3f::Back() * 10.0F * deltaTime * trans.rotation);
            }

            if (Input::InputManager::IsKeyDown(Input::Key::D)) {
                auto &trans = registry.GetSingleton<Camera>().transform;
                trans.Translate(Vector3f::Right() * 10.0F * deltaTime * trans.rotation);
            }

            if (Input::InputManager::IsKeyDown(Input::Key::A)) {
                auto &trans = registry.GetSingleton<Camera>().transform;
                trans.Translate(Vector3f::Left() * 10.0F * deltaTime * trans.rotation);
            }

            if (Input::InputManager::IsKeyDown(Input::Key::LShift)) {
                registry.GetSingleton<Camera>().transform.Translate(Vector3f::Up() * 10.0F * deltaTime);
            }

            if (Input::InputManager::IsKeyDown(Input::Key::LCtrl)) {
                registry.GetSingleton<Camera>().transform.Translate(Vector3f::Down() * 10.0F * deltaTime);
            }

            if (Input::InputManager::IsKeyDown(Input::Key::E)) {
                registry.GetSingleton<Camera>().transform.Rotate(deltaTime * 100.0F, Vector3f::Up());
            }

            if (Input::InputManager::IsKeyDown(Input::Key::Q)) {
                registry.GetSingleton<Camera>().transform.Rotate(-deltaTime * 100.0F, Vector3f::Up());
            }
        }
    };

    void EditorRender::Render(Application::Context &context, double deltaTime) {
        auto &[registry, systemRegistry] = context.worldManager.GetActiveWorld();

        const Gl::Viewport viewport{.aspect = context.window.GetSize()};

        auto &cam = registry.GetSingleton<Camera>();
        cam.aspectRatio = context.window.GetAspectRatio();

        std::vector<PointLight> lights;
        registry.ForEach<const PointLight>([&](const PointLight &light) { lights.push_back(light); });

        Renderer &renderer = context.renderer;
        renderer.BeginPass({.viewMatrix = cam.GetViewMatrix(), .projectionMatrix = cam.GetProjMatrix()});
        renderer.AddLights({.ambientLight = registry.GetSingleton<AmbientLight>(), .pointLights = lights});

        registry.ForEach<const ModelRenderer, const Transform>(
                [&](const ModelRenderer &model, const Transform &transform) {
                    for (auto &[mesh, material]: context.assetManager.GetModel(model.assetHandle).elements) {
                        renderer.Submit({.mesh = mesh, .material = material, .modelMatrix = transform.GetMatrix()});
                    }
                });

        renderer.Render(viewport, context.resources);
    }

    void EditorGui::RenderGui(Application::Context &context, double deltaTime) { RenderOutline(context); }

    void EditorGui::RenderOutline(Application::Context &context) {
        Gui::BeginWindow("Outline");
        if (Gui::Button("Button")) {
            PIXF_CORE_LOG_DEBUG("Button pressed");
        }

        Gui::EndWindow();
    }

    class App final : public Application::Application {
    public:
        App() : Application({.windowConfig = {.title = "Editor", .size = {1600, 900}}}) {}

    protected:
        void Awake() override {
            Logger::Get("Core").GetConfig().visibility &= ~LogLevelTrace;

            const Blueprint blueprint([](World &world) {
                PIXF_REGISTER_COMP(PointLight, world.registry);
                PIXF_REGISTER_COMP(Transform, world.registry);
                PIXF_REGISTER_COMP(ModelRenderer, world.registry);

                world.registry.CreateSingleton<Camera>();
                world.registry.CreateSingleton<AmbientLight>();

                world.systemRegistry.Register<InputSystem>();
            });

            const auto idx = GetContext().worldManager.LoadWorld(blueprint, "world.json");
            GetContext().worldManager.SetActive(idx);

            GetContext().assetManager.ImportAll();
            GetContext().assetManager.LoadAll(GetContext().resources);

            AttachStage<EditorRender>();
            AttachStage<EditorGui>();
        }

        void Update(const double deltaTime) override {
            auto &[registry, systemRegistry] = GetContext().worldManager.GetActiveWorld();
            systemRegistry.Update(registry, deltaTime);
        }
    };
} // namespace Pixf::Editor

PIXF_RUN_APPLICATION(Pixf::Editor::App)
