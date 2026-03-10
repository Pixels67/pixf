#include "App.hpp"

#include "Audio/AudioSource.hpp"
#include "Graphics/ModelRenderer.hpp"
#include "Input/Input.hpp"
#include "Math/Transform.hpp"
#include "Serial/JsonArchive.hpp"
#include "Time/Time.hpp"

namespace Flock {
    std::optional<App> App::Create(const AppConfig &config) {
        App app;
        app.m_Config = config;
        auto window  = Glfw::Window::Create(config.windowConfig);
        if (!window) {
            Debug::LogErr("App::Create: Failed to create window!");
            return std::nullopt;
        }

        app.m_Services.window = std::move(window.value());
        app.m_Services.window.MakeCurrent();

        auto audioPlayer = Audio::AudioPlayer::Create();
        if (!audioPlayer) {
            Debug::LogErr("App::Create: Failed to create audio player!");
            return std::nullopt;
        }

        app.m_Services.audioPlayer   = std::move(audioPlayer.value());
        app.m_Services.physicsEngine = std::move(Physics::PhysicsEngine::Create());

        const auto pipeline = app.m_Services.assetLoader.Load<Graphics::Pipeline>("../../../assets/shader.glsl");
        if (!pipeline) {
            Debug::LogErr("App::Create: Could not load shader file 'shader.glsl'!");
            return std::nullopt;
        }

        app.m_Services.assetLoader.SetDefaultPipeline(Asset::PipelineType::Pbr, pipeline.value());

        return app;
    }

    App::~App() {
        m_Services.assetLoader.UnloadAll();
    }

    App &App::AddSystem(const Ecs::Stage stage, const Ecs::System &system) {
        m_Schedule.AddSystem(stage, system);

        return *this;
    }

    App &App::PopSystem(const Ecs::Stage stage) {
        m_Schedule.PopSystem(stage);

        return *this;
    }

    App &App::Run() {
        m_Services.window.MakeCurrent();
        m_Services.inputHandler.HookEvents(m_Services.eventHandler);

        m_World.InsertResource<Time::TimeState>();
        m_World.InsertResource<Input::InputState>();
        m_World.InsertResource<Graphics::Camera>();
        m_World.InsertResource<Graphics::AmbientLight>();
        m_World.InsertResource<Audio::AudioListener>();

        m_Schedule.Execute(Ecs::Stage::Startup, m_World);

        while (!m_Services.window.ShouldClose()) {
            // Begin
            m_Services.window.PollEvents(m_Services.eventHandler);
            m_Services.eventHandler.Update();

            // Update
            Prepare();
            m_Schedule.Execute(Ecs::Stage::Update, m_World);
            Extract();

            // Render
            Render(m_Config.shadowConfig);

            // Finish
            m_Services.window.SwapBuffers();
            m_Services.inputHandler.ResetState();
        }

        return *this;
    }

    Services &App::GetServices() {
        return m_Services;
    }

    void App::Prepare() {
        const f64 deltaTime = Time::GetTime() - m_World.GetResource<Time::TimeState>().time;

        m_World.InsertResource(Time::TimeState{.time = Time::GetTime(), .deltaTime = deltaTime});
        m_World.InsertResource(m_Services.inputHandler.GetState());
    }

    void App::Extract() {
        const auto listener = m_World.GetResource<Audio::AudioListener>();
        m_Services.audioPlayer.SetListener(listener);

        std::vector<Audio::AudioSource> sources;
        m_World.GetRegistry().ForEach<Audio::AudioSource>([&](Audio::AudioSource &source) {
            if (!source.play) {
                return;
            }

            source.play = false;

            const auto clip = m_Services.assetLoader.Load<Audio::AudioClip>(source.audioClipPath);
            if (!clip) {
                Debug::LogErr("App::Extract: Invalid AudioSource clip path '{}'!", source.audioClipPath);
                return;
            }

            const Audio::AudioConfig config = {
                .position   = source.position,
                .volume     = source.volume,
                .pitch      = source.pitch,
                .pan        = source.pan,
                .looping    = source.looping,
                .spatialize = source.spatialize
            };

            m_Services.audioPlayer.Play(m_Services.assetLoader.Get(clip.value()).value(), config);
        });

        std::vector<Physics::PhysicsObject> physicsObjects;
        m_World.GetRegistry().ForEach<Transform, Physics::BoxCollider, Physics::RigidBody>(
            [&](Transform &trans, Physics::BoxCollider &collider, Physics::RigidBody &rb) {
                physicsObjects.push_back({.transform = &trans, .rigidBody = &rb, .collider = &collider});
            }
        );

        m_World.GetRegistry().ForEach<Transform, Physics::SphereCollider, Physics::RigidBody>(
            [&](Transform &trans, Physics::SphereCollider &collider, Physics::RigidBody &rb) {
                physicsObjects.push_back({.transform = &trans, .rigidBody = &rb, .collider = &collider});
            }
        );

        m_Services.physicsEngine.SetScene(physicsObjects);
        m_Services.physicsEngine.Update(m_World.GetResource<Time::TimeState>().deltaTime);
    }

    void App::Render(const Graphics::ShadowConfig shadowConfig) {
        using namespace Graphics;

        const Camera       camera   = m_World.GetResource<Camera>();
        const Rect2u       viewport = {{0, 0}, m_Services.window.GetSize()};
        std::vector<Light> lights;
        m_World.GetRegistry().ForEach<Light>([&](auto &light) {
            lights.push_back(light);
        });

        const AmbientLight ambient = m_World.GetResource<AmbientLight>();

        SceneData scene = {
            .camera       = camera,
            .lights       = lights,
            .ambientLight = ambient,
        };

        m_Services.renderer.BeginPass({.viewport = viewport});

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

        m_Services.renderer.Render(scene, shadowConfig, m_Services.assetLoader);
    }
}
