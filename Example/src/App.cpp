#include "Application/Application.hpp"
#include "Debug/Logger.hpp"
#include "Entities/Components/Graphics/Camera.hpp"
#include "Entities/Components/Graphics/ModelRenderer.hpp"
#include "Entities/Components/Graphics/PointLight.hpp"
#include "Entities/Components/Transform.hpp"
#include "Entities/System.hpp"
#include "Files/Assets/AssetManager.hpp"
#include "Files/Model.hpp"
#include "Graphics/Gl/Viewport.hpp"
#include "Graphics/Gl/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Gui/Gui.hpp"
#include "Serial/JsonArchive.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Math;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Debug;
using namespace Pixf::Core::Entities;
using namespace Pixf::Core::Entities::Components;
using namespace Pixf::Core::Entities::Components::Graphics;

struct RotateSystem final : System {
    void Update(Registry &registry, const double deltaTime) override {
        registry.ForEach<Transform>([deltaTime](Transform &transform) {
            transform.Rotate(360.0F * deltaTime, Vector3f::Up());
            transform.Translate(Vector3f(0.0F, 0.0F, 5.0F * deltaTime));
        });
    }
};

class RenderStage final : public Application::Stage {
public:
    void OnAttach(Application::Context &context) override {
        const Blueprint blueprint([](World &world) {
            PIXF_REGISTER_COMP(PointLight, world.registry);
            PIXF_REGISTER_COMP(Transform, world.registry);
            PIXF_REGISTER_COMP(ModelRenderer, world.registry);

            world.registry.CreateSingleton<Camera>();
            world.registry.CreateSingleton<AmbientLight>();

            world.systemRegistry.Register<RotateSystem>();
        });

        const auto idx = context.worldManager.LoadWorld(blueprint, "world.json");
        context.worldManager.SetActive(idx);

        const auto uuid = Uuid::Uuid::FromString("03afb080-1f81-5ed9-80da-0bbe2a5d1b22").value();
        context.assetManager.ImportModel(uuid, context.resources);
    }

    void Update(Application::Context &context, const double deltaTime) override {
        auto &[registry, systemRegistry] = context.worldManager.GetActiveWorld();
        systemRegistry.Update(registry, deltaTime);
    }

    void Render(Application::Context &context, double deltaTime) override {
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
                    for (auto &[mesh, material]: context.assetManager.GetModel(model.uuid).elements) {
                        renderer.Submit({.mesh = mesh, .material = material, .modelMatrix = transform.GetMatrix()});
                    }
                });

        renderer.Render(viewport, context.resources);
    }
};

class App final : public Application::Application {
protected:
    void Awake() override {
        Logger::Get("Core").GetConfig().visibility &= ~LogLevelTrace;
        AttachStage<RenderStage>();
    }

    void RenderGui(double deltaTime) override {
        Gui::BeginWindow("Window");
        Gui::BeginChild("Timmy", {300, 400});
        Gui::ColoredText({255, 0, 255, 255}, "Test: {}, {}", 5.76, true);
        if (Gui::Button("Button")) {
            PIXF_CORE_LOG_DEBUG("Button pressed");
        }

        Vector3f vec;
        Gui::DragVector3f("drag", vec);

        Gui::EndChild();
        Gui::EndWindow();
    }
};

PIXF_CREATE_APPLICATION(App)
