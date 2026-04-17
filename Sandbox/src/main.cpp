#include "Flock.hpp"

using namespace Flock;
using namespace Flock::Ecs;
using namespace Flock::Graphics;
using namespace Flock::Input;
using namespace Flock::Audio;

i32 main() {
    App app = App::Create({
        .windowConfig = {.size = {1080, 800}},
    }).value();

    app.AddSystem(Stage::Startup, [](World &world) {
        const auto &assets = world.Resource<Asset::Assets>();

        assets.SetPipeline("PBR", "../../../assets/shader.glsl");
        assets.Get<Model>("../../../assets/box.glb")->objects[0].material = {
            .colorMapPath = "../../../assets/Checkerboard.png"
        };

        assets.Get<Texture>("../../../assets/Checkerboard.png")->Configure({
            .wrapMode = Clamp,
        });

        auto &reg = world.Registry();

        world.Resource<Camera>().projection         = Projection::Perspective;
        world.Resource<Camera>().transform.position = {0.0F, -8.0F, -32.0F};

        world.Resource<AmbientLight>().color = {20, 20, 20};
        world.Resource<Skybox>().filePath    = "../../../assets/sky.png";

        for (f32 i = -12.0F; i <= 12.0F; i += 4.0F) {
            for (f32 j = -12.0F; j <= 12.0F; j += 4.0F) {
                for (f32 k = -12.0F; k <= 12.0F; k += 4.0F) {
                    reg.Create(
                        Transform{
                            .position = {i, j, k},
                        },
                        ModelRenderer{.model = assets.loader.Load<Model>("../../../assets/box.glb")},
                        Physics::BoxCollider{{}, Vector3f::One()},
                        Physics::RigidBody{.linearVelocity = {-i / 2, -j / 2, -k / 2}}
                    );
                }
            }
        }

        Model model;
        model.Add(Mesh::Box(), {});

        reg.Create(
            Transform{
                .position = {0.0F, -20.0F, 0.0F},
                .scale    = {100.0F, 0.5F, 100.0F}
            },
            ModelRenderer{.model = assets.loader.Register<Model>(std::move(model))},
            Physics::BoxCollider{},
            Physics::RigidBody{.mode = Physics::SimulationMode::Static}
        );

        reg.Create(DirectionalLight{
            .position  = {-4.0F, 5.0F, -3.0F},
            .color     = {255, 255, 220},
            .intensity = 5.0F,
        });

        world.Resource<InputState>().cursorMode = CursorMode::Disabled;

        world.Save("../../../assets/world.json");
    });

    app.AddSystem(Stage::Update, [&](World &world) {
        const f64 dt = world.Resource<Time::TimeState>().deltaTime;

        auto &input = world.Resource<InputState>();
        auto &cam   = world.Resource<Camera>();

        const f32     moveSpeed = 5.0F * dt;
        constexpr f32 rotSpeed  = 0.4F;

        if (input.IsKeyDown(Key::Escape)) {
            input.cursorMode = CursorMode::Normal;
        }
        if (input.IsMouseDown()) {
            input.cursorMode = CursorMode::Disabled;
        }

        if (input.IsKeyDown(Key::W)) {
            cam.transform.position += Vector3f::Forward() * moveSpeed * cam.transform.rotation;
        }
        if (input.IsKeyDown(Key::S)) {
            cam.transform.position -= Vector3f::Forward() * moveSpeed * cam.transform.rotation;
        }

        if (input.IsKeyDown(Key::D)) {
            cam.transform.position += Vector3f::Right() * moveSpeed * cam.transform.rotation;
        }
        if (input.IsKeyDown(Key::A)) {
            cam.transform.position -= Vector3f::Right() * moveSpeed * cam.transform.rotation;
        }

        if (input.IsKeyDown(Key::LShift)) {
            cam.transform.position += Vector3f::Up() * moveSpeed * cam.transform.rotation;
        }
        if (input.IsKeyDown(Key::LControl)) {
            cam.transform.position -= Vector3f::Up() * moveSpeed * cam.transform.rotation;
        }

        const Vector2f mouseDelta = input.CursorDelta();

        static f32 pitchAngle = 0.0F;
        static f32 yawAngle   = 0.0F;

        pitchAngle += mouseDelta.y * rotSpeed;
        pitchAngle = std::clamp(pitchAngle, -89.0f, 89.0f);
        yawAngle   += mouseDelta.x * rotSpeed;

        cam.transform.rotation = Quaternion::Euler(pitchAngle, 0.0F, 0.0F);
        cam.transform.rotation *= Quaternion::Euler(0.0F, yawAngle, 0.0F);
    });

    app.Run();
}
