#include "App.hpp"

#include "Graphics/ModelRenderer.hpp"
#include "Input/Input.hpp"
#include "Math/Transform.hpp"

namespace Flock {
    App &App::AddSystem(const Ecs::Stage stage, const Ecs::System &system) {
        m_Schedule.AddSystem(stage, system);

        return *this;
    }

    App &App::PopSystem(const Ecs::Stage stage) {
        m_Schedule.PopSystem(stage);

        return *this;
    }

    App &App::Run() {
        auto window = Glfw::Window::Create({.size = {800, 800}});
        if (!window) {
            Debug::LogErr("App::Run: Failed to create window!");
            return *this;
        }

        window.value().MakeCurrent();
        m_Services.window = std::move(window.value());

        m_Services.inputHandler.HookEvents(m_Services.eventHandler);
        m_World.InsertResource<Input::InputState>();
        m_World.InsertResource<Graphics::Camera>({.projection = Graphics::Projection::Perspective});
        m_Schedule.Execute(Ecs::Stage::Startup, m_World);

        Graphics::RenderConfig config = {
            .camera = {
                .projection = Graphics::Projection::Perspective,
            },
            .viewport         = {{0, 0}, m_Services.window.GetSize()},
            .ambientIntensity = 0.1F,
            .lights           = {
                {
                    .position  = {0.5F, 1.0F, -0.5F},
                    .color     = {255, 220, 40},
                    .intensity = 10.0F,
                    .isStatic  = true,
                },
                {
                    .position  = {-0.5F, 1.0F, 0.5F},
                    .color     = {40, 220, 255},
                    .intensity = 10.0F,
                    .isStatic  = true,
                },
            },
        };

        while (!m_Services.window.ShouldClose()) {
            m_Services.window.PollEvents(m_Services.eventHandler);
            m_Services.eventHandler.Update();
            m_World.InsertResource(m_Services.inputHandler.GetState());

            m_Schedule.Execute(Ecs::Stage::Update, m_World);

            config.camera = m_World.GetResource<Graphics::Camera>();
            Render(config);
            m_Services.window.SwapBuffers();
            m_Services.inputHandler.ResetState();
        }

        return *this;
    }

    Services &App::GetServices() {
        return m_Services;
    }

    void App::Render(Graphics::RenderConfig config) {
        using namespace Graphics;

        const auto pipeline = m_Services.assetLoader.Load<Pipeline>("../../../assets/shader.glsl");
        if (!pipeline) {
            Debug::LogErr("App::Render: Could not find shader file 'shader.glsl'!");
            return;
        }

        m_Services.assetLoader.SetDefaultPipeline(Asset::PipelineType::Pbr, pipeline.value());
        m_Services.renderer.BeginPass(config);

        m_World.GetRegistry().ForEach<ModelRenderer, Transform>(
            [&](const ModelRenderer &renderer, const Transform &transform) {
                const auto result = m_Services.assetLoader.Load<Model>(renderer.modelPath);
                if (!result) {
                    Debug::LogErr("App::Render: Invalid model path!");
                    return;
                }

                Model &model = m_Services.assetLoader.Get(result.value()).value();

                for (usize i = 0; i < model.meshes.size(); i++) {
                    m_Services.renderer.Submit({
                        .mesh      = &model.meshes[i],
                        .material  = model.materials[i],
                        .transform = transform,
                    });
                }
            });

        m_Services.renderer.Render(m_Services.assetLoader);
    }
}
