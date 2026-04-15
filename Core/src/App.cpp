#include "App.hpp"

#include <filesystem>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>

#include "Asset/Assets.hpp"
#include "Audio/AudioSource.hpp"
#include "Event/EventRegistry.hpp"
#include "Graphics/ModelRenderer.hpp"
#include "Graphics/Skybox.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Gui/Box.hpp"
#include "Gui/Button.hpp"
#include "Gui/Image.hpp"
#include "Gui/Text.hpp"
#include "Input/Input.hpp"
#include "Math/Transform.hpp"
#include "Time/Time.hpp"
#include "Audio/AudioClip.hpp"
#include "Audio/AudioListener.hpp"
#include "Debug/Log.hpp"
#include "Ecs/Registry.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CubeMap.hpp"
#include "Graphics/Light.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/Texture.hpp"
#include "Gui/Font.hpp"
#include "Gui/RectTransform.hpp"
#include "Math/Color.hpp"
#include "Math/Rect.hpp"
#include "Math/Vector.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"

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
        app.m_Services.guiRenderer   = std::move(Gui::GuiRenderer::Create());

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

        m_World = Ecs::World::Default();
        m_World.InsertResource<Asset::Assets>(Asset::Assets{m_Services.assetLoader});
        m_Schedule.Execute(Ecs::Stage::Startup, m_World);

        while (!m_Services.window.ShouldClose() && !m_ShouldClose) {
            // Begin
            m_Services.window.PollEvents(m_Services.eventHandler);
            m_Services.eventHandler.Update();

            // Update
            Prepare();
            m_Schedule.Execute(Ecs::Stage::Update, m_World);
            Extract();

            // Render
            Render();
            RenderGui();

            // Finish
            m_Services.window.SwapBuffers();
            m_Services.inputHandler.ResetState();
        }

        return *this;
    }

    Services &App::Services() {
        return m_Services;
    }

    void App::Prepare() {
        const f64 deltaTime = Time::Time() - m_World.Resource<Time::TimeState>().time;

        m_World.InsertResource(Time::TimeState{.time = Time::Time(), .deltaTime = deltaTime});

        Input::InputState input = m_Services.inputHandler.State();
        input.cursorMode        = m_World.Resource<Input::InputState>().cursorMode;
        m_World.InsertResource(input);
    }

    void App::Extract() {
        m_Services.window.SetCursorMode(m_World.Resource<Input::InputState>().cursorMode);

        const auto listener = m_World.Resource<Audio::AudioListener>();
        m_Services.audioPlayer.SetListener(listener);

        std::vector<Audio::AudioSource> sources;
        m_World.Registry().ForEach<Audio::AudioSource>([&](Audio::AudioSource &source) {
            if (source.audioClipPath.empty()) {
                return;
            }

            const auto clip = m_Services.assetLoader.Get<Audio::AudioClip>(source.audioClipPath);
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

            if (clip.value().get().playbackHandle != FLK_INVALID) {
                m_Services.audioPlayer.Configure(clip->get(), config);
            }

            if (source.play) {
                source.play = false;
                m_Services.audioPlayer.Play(clip.value(), config);
            }

            if (source.stop) {
                source.stop = false;
                m_Services.audioPlayer.Stop(clip.value());
            }
        });

        std::vector<Physics::PhysicsObject> physicsObjects;
        m_World.Registry().ForEach<Transform, Physics::BoxCollider, Physics::RigidBody>(
            [&](Transform &trans, Physics::BoxCollider &collider, Physics::RigidBody &rb) {
                physicsObjects.push_back({.transform = &trans, .rigidBody = &rb, .collider = &collider});
            }
        );

        m_World.Registry().ForEach<Transform, Physics::SphereCollider, Physics::RigidBody>(
            [&](Transform &trans, Physics::SphereCollider &collider, Physics::RigidBody &rb) {
                physicsObjects.push_back({.transform = &trans, .rigidBody = &rb, .collider = &collider});
            }
        );

        static f32 accumulator = 0.0F;
        accumulator            += m_World.Resource<Time::TimeState>().deltaTime;

        m_Services.physicsEngine.SetScene(physicsObjects);

        while (accumulator >= 0.02F) {
            m_Services.physicsEngine.Update(0.02F);
            accumulator -= 0.02F;
        }

        m_ShouldClose = m_World.Resource<Application>().shouldClose;
    }

    void App::Render() {
        using namespace Graphics;

        const Camera       camera = m_World.Resource<Camera>();
        std::vector<Light> lights;

        m_World.Registry().ForEach<Light>([&](auto &light) {
            lights.push_back(light);
        });

        m_World.Registry().ForEach<DirectionalLight>([&](auto &light) {
            lights.push_back(light.Light());
        });

        m_World.Registry().ForEach<PointLight>([&](auto &light) {
            lights.push_back(light.Light());
        });

        const AmbientLight ambient = m_World.Resource<AmbientLight>();
        const Skybox       skybox  = m_World.Resource<Skybox>();

        OptionalRef<CubeMap> cubeMap = std::nullopt;
        if (m_Services.assetLoader.Get<CubeMap>(skybox.filePath) && camera.projection == Projection::Perspective) {
            cubeMap = m_Services.assetLoader.Get<CubeMap>(skybox.filePath)->get();
        }

        SceneData scene = {
            .camera       = camera,
            .lights       = lights,
            .ambientLight = ambient,
            .skybox       = cubeMap
        };

        RenderList commands;
        m_World.Registry().ForEach<ModelRenderer, Transform>([&](const ModelRenderer &renderer, const Transform &transform) {
            const auto result = m_Services.assetLoader.Get<Model>(renderer.modelPath);
            if (!result) {
                Debug::LogErr("App::Render: Invalid model path!");
                return;
            }

            Model &model = result.value();
            for (auto &[mesh, mat]: model.objects) {
                MaterialProperties props = {
                    .color     = mat.color,
                    .metallic  = mat.metallic,
                    .roughness = mat.roughness,
                };

                if (mat.colorMapPath != "") {
                    props.colorMap = m_Services.assetLoader.Get<Texture>(mat.colorMapPath);
                }

                if (mat.metallicMapPath != "") {
                    props.metallicMap = m_Services.assetLoader.Get<Texture>(mat.metallicMapPath);
                }

                if (mat.roughnessMapPath != "") {
                    props.roughnessMap = m_Services.assetLoader.Get<Texture>(mat.roughnessMapPath);
                }

                commands.push_back({
                    .mesh               = mesh,
                    .pipeline           = m_Services.assetLoader.Get<Pipeline>(mat.pipelinePath).value(),
                    .materialProperties = props,
                    .transform          = transform,
                });
            }
        });

        const Rect2u viewport = {{0, 0}, m_Services.window.Size()};
        m_Services.renderer.Render(
            commands,
            scene,
            {
                .viewport = viewport,
                .clear    = {.color = Color4u8{ambient.color}}
            },
            m_Config.shadowConfig
        );

        commands.clear();

        Mesh square = Mesh::Square();
        auto unlit  = m_Services.assetLoader.Get<Pipeline>("@Unlit");
        if (!unlit) {
            Debug::LogWrn("App::Render: Unlit pipeline not assigned, 2D objects won't be rendered.");
        } else {
            m_World.Registry().ForEach<SpriteRenderer, Transform>([&](const SpriteRenderer &renderer, const Transform &transform) {
                MaterialProperties props = {
                    .color     = renderer.color,
                    .metallic  = 0.0F,
                    .roughness = 1.0F,
                };

                const auto result = m_Services.assetLoader.Get<Texture>(renderer.spritePath);
                if (result.has_value()) {
                    Texture &sprite = result.value();
                    props.colorMap  = sprite;
                }

                commands.push_back({
                    .mesh               = square,
                    .pipeline           = unlit->get(),
                    .materialProperties = props,
                    .transform          = transform,
                });
            });
        }

        scene.skybox = std::nullopt;
        scene.lights = {};
        m_Services.renderer.Render(
            commands,
            scene,
            {
                .viewport = viewport,
                .clear    = {.clearColor = false, .clearDepth = false}
            }
        );
    }

    void App::RenderGui() {
        using namespace Gui;

        const auto input = m_World.Resource<Input::InputState>();

        const bool mouseDown     = input.IsMouseDown();
        const bool mouseReleased = input.IsMouseReleased();
        const bool mousePressed  = input.IsMousePressed();

        m_Services.guiRenderer.BeginFrame(m_Services.window.Size());

        m_World.Registry().ForEach<RectTransform, Box>([&](const RectTransform &trans, const Box &box) {
            m_Services.guiRenderer.RenderRect(
                trans,
                box.color
            );
        });

        m_World.Registry().ForEach<RectTransform, Image>([&](const RectTransform &trans, const Image &img) {
            if (img.imagePath.empty() || !m_Services.assetLoader.Get<Graphics::Texture>(img.imagePath)) {
                m_Services.guiRenderer.RenderRect(trans, Color4u8::White());
                return;
            }

            m_Services.guiRenderer.RenderImage(trans, m_Services.assetLoader.Get<Graphics::Texture>(img.imagePath).value());
        });

        m_World.Registry().ForEach<RectTransform, Button>([&](const RectTransform &trans, const Button &button) {
            Color4u8 tint = Color4u8::Transparent();

            if (input.IsCursorInRect(trans.rect) && mouseDown) {
                tint = button.pressTint;
            } else if (input.IsCursorInRect(trans.rect)) {
                tint = button.hoverTint;
            }

            OptionalRef<Graphics::Texture> tex = std::nullopt;

            if (button.imagePath != "" && m_Services.assetLoader.Get<Graphics::Texture>(button.imagePath)) {
                tex = m_Services.assetLoader.Get<Graphics::Texture>(button.imagePath).value();
            }

            m_Services.guiRenderer.RenderButton(
                trans,
                button.defaultColor,
                tint,
                tex
            );

            const auto events = m_World.Resource<Event::EventRegistry>();

            if (input.IsCursorInRect(trans.rect) && mousePressed) {
                events.Invoke(button.onPressEvent);
            }

            if (input.IsCursorInRect(trans.rect) && mouseReleased) {
                events.Invoke(button.onReleaseEvent);
            }
        });

        m_World.Registry().ForEach<RectTransform, Text>([&](const RectTransform &trans, const Text &text) {
            const auto font = m_Services.assetLoader.Get<Font>(text.fontPath);
            if (!font) {
                Debug::LogErr("App::RenderGui: Invalid font path '{}'", text.fontPath);
            }

            m_Services.guiRenderer.RenderText(
                text.content,
                text.fontSize,
                font.value(),
                trans,
                text.color,
                text.horizontalAlignment,
                text.verticalAlignment
            );
        });

        m_Services.guiRenderer.EndFrame();
    }
}
