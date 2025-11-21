#include "Application/Application.hpp"
#include "Entities/System.hpp"
#include "Files/Model.hpp"
#include "Graphics/Gl/Viewport.hpp"
#include "Graphics/Renderer.hpp"
#include "Gui/Gui.hpp"
#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Math;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Debug;

struct Backpack {
    Matrix4f transform{};
    Model backpack;
    float dist = 0.0F;
    float rot = 0.0F;
};

struct BackpackSystem final : Entities::System {
    void Update(Entities::Registry &registry, const double deltaTime) override {
        registry.ForEach<Backpack>([deltaTime](Backpack &backpack) {
            backpack.dist += 5.0F * deltaTime;
            backpack.rot += 5.0F * deltaTime;

            backpack.transform = Matrix4f::Identity();
            backpack.transform.ApplyRotation(backpack.rot, Vector3f::Up());
            backpack.transform.ApplyTranslation(Vector3f(0.0F, 0.0F, backpack.dist));
        });
    }
};

class RenderStage final : public Application::Stage {
public:
    void OnAttach(Application::State &state) override {
        const auto entity = state.entityRegistry.CreateEntity();
        const Backpack backpack{.backpack = Files::LoadModel("backpack.obj", state.resources)};

        state.entityRegistry.AddComponent(entity, backpack);
        state.systemRegistry.Register<BackpackSystem>();
    }

    void Update(Application::State &state, const double deltaTime) override {
        state.systemRegistry.Update(state.entityRegistry, deltaTime);
    }

    void Render(Application::State &state, double deltaTime) override {
        const Gl::Viewport viewport{.aspect = state.window.GetSize()};

        Renderer &renderer = state.renderer;
        renderer.BeginPass({.projectionMatrix = Matrix4f::Perspective(60.0F, 4.0F / 3.0F, 0.1F, 100.0F)});

        state.entityRegistry.ForEach<Backpack>([&renderer](Backpack &backpack) {
            for (auto &[mesh, material]: backpack.backpack.elements) {
                renderer.Submit({.mesh = mesh, .material = material, .modelMatrix = backpack.transform});
            }
        });

        renderer.Render(viewport, state.resources);
    }
};

class App final : public Application::Application {
protected:
    void Awake() override { AttachStage<RenderStage>(); }

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

PIXF_RUN_APPLICATION(App)
