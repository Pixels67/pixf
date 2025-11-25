#include "Application/Application.hpp"
#include "Debug/Logger.hpp"
#include "Entities/Components/Graphics/ModelRenderer.hpp"
#include "Entities/Components/RigidTransform.hpp"
#include "Entities/Components/Transform.hpp"
#include "Entities/System.hpp"
#include "Files/Assets/AssetManager.hpp"
#include "Files/File.hpp"
#include "Files/Model.hpp"
#include "Graphics/Gl/Gl.hpp"
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
using namespace Pixf::Core::Entities::Graphics;

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
    void OnAttach(Application::State &state) override {
        PIXF_REGISTER_COMP(Transform, state.entityRegistry);
        PIXF_REGISTER_COMP(ModelRenderer, state.entityRegistry);

        auto entity = state.entityRegistry.CreateEntity();

        // ModelRenderer model;
        // model.uuid = Uuid::Uuid::FromString("03afb080-1f81-5ed9-80da-0bbe2a5d1b22").value();
        // state.entityRegistry.AddComponent(entity, model);
        // state.entityRegistry.AddComponent<Transform>(entity);
        // Serial::JsonOutputArchive archive;
        // archive("registry", state.entityRegistry);
        // Files::WriteFile("reg.json", archive.Get().ToString());

        const Json::Json json = Json::Json::Parse(Files::ReadFile("reg.json"));
        Serial::JsonInputArchive archive(json);
        archive("registry", state.entityRegistry);

        state.systemRegistry.Register<RotateSystem>();
        state.assetManager.ImportModel(Uuid::Uuid::FromString("03afb080-1f81-5ed9-80da-0bbe2a5d1b22").value(),
                                       state.resources);
    }

    void Update(Application::State &state, const double deltaTime) override {
        state.systemRegistry.Update(state.entityRegistry, deltaTime);
    }

    void Render(Application::State &state, double deltaTime) override {
        const Gl::Viewport viewport{.aspect = state.window.GetSize()};

        Renderer &renderer = state.renderer;
        renderer.BeginPass({.projectionMatrix = Matrix4f::Perspective(60.0F, 4.0F / 3.0F, 0.1F, 100.0F)});

        state.entityRegistry.ForEach<const ModelRenderer, const Transform>(
                [&](const ModelRenderer &model, const Transform &transform) {
                    for (auto &[mesh, material]: state.assetManager.GetModel(model.uuid).elements) {
                        renderer.Submit({.mesh = mesh, .material = material, .modelMatrix = transform.GetMatrix()});
                    }
                });

        renderer.Render(viewport, state.resources);
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
