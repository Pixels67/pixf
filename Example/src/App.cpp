#include "Application/Application.hpp"
#include "Files/Model.hpp"
#include "Graphics/Gl/Viewport.hpp"
#include "Graphics/Renderer.hpp"
#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Math;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Debug;

class RenderStage final : public Application::Stage {
public:
    void OnAttach(Application::State &state) override { backpack = Files::LoadModel("backpack.obj", state.resources); }

    void Update(Application::State &state, const double deltaTime) override {
        dist += 5.0F * deltaTime;
        rot += 5.0F * deltaTime;

        transform = Matrix4f::Identity();
        transform.ApplyRotation(rot, Vector3f::Up());
        transform.ApplyTranslation(Vector3f(0.0F, 0.0F, dist));
    }

    void Render(Application::State &state, double deltaTime) override {
        const Gl::Viewport viewport{.aspect = state.window.GetSize()};

        Renderer &renderer = state.renderer;
        renderer.BeginPass({.projectionMatrix = Matrix4f::Perspective(60.0F, 4.0F / 3.0F, 0.1F, 100.0F)});

        for (auto &[mesh, material]: backpack.elements) {
            renderer.Submit({.mesh = mesh, .material = material, .modelMatrix = transform});
        }

        renderer.Render(viewport, state.resources);
    }

private:
    Matrix4f transform;
    Model backpack;
    float dist = 0.0F;
    float rot = 0.0F;
};

class App final : public Application::Application {
protected:
    void Awake() override { AttachStage<RenderStage>(); }
};

PIXF_RUN_APPLICATION(App)
