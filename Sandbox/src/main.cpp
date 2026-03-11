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
        const auto &assets = world.GetResource<Asset::Assets>();

        assets.SetDefaultPipeline(Asset::PipelineType::Pbr, "../../../assets/shader.glsl");
        assets.Load<Model>("../../../assets/FlightHelmet.gltf");
        assets.Load<Model>("../../../assets/box.glb");
        assets.Load<Model>("../../../assets/sphere.glb");

        assets.Get<Model>("../../../assets/box.glb").objects[0].material = {
            .colorMapPath = "../../../assets/Checkerboard.png"
        };

        assets.Get<Texture>("../../../assets/Checkerboard.png").Configure({
            .filterMode = Nearest
        });

        auto &reg = world.GetRegistry();

        world.GetResource<Camera>().projection         = Projection::Perspective;
        world.GetResource<Camera>().transform.position = {0.0F, 2.0F, -12.0F};
        reg.Create(
            Transform{
                .position = {0.0F, 5.0F, 0.0F},
                .rotation = Quaternion::Euler(40.0F, 20.0F, 10.0F),
                .scale    = Vector3f::One() * 5.0F
            },
            ModelRenderer{.modelPath = "../../../assets/FlightHelmet.gltf"},
            Physics::BoxCollider({.position = {0.0F, 1.5F, 0.0F}}, Vector3f::One() * 0.2F + Vector3f::Up() * 0.1F),
            Physics::RigidBody{.mass = 1.0F}
        );

        reg.Create(
            Transform{
                .position = {0.0F, -0.5F, 0.0F},
                .rotation = Quaternion::Euler(0.0F, 0.0F, 0.0F),
                .scale    = {10.0F, 0.5F, 10.0F}
            },
            ModelRenderer{.modelPath = "../../../assets/box.glb"},
            Physics::BoxCollider{},
            Physics::RigidBody{.mode = Physics::SimulationMode::Static}
        );

        reg.Create(DirectionalLight{
            .position  = {-4.0F, 5.0F, -3.0F},
            .color     = {255, 255, 220},
            .intensity = 5.0F,
        });

        world.GetResource<InputState>().cursorMode = CursorMode::Disabled;
    }).AddSystem(Stage::Update, [&](World &world) {
        const f64 dt = world.GetResource<Time::TimeState>().deltaTime;

        auto &input = world.GetResource<InputState>();
        auto &cam   = world.GetResource<Camera>();

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

        const Vector2f mouseDelta = input.GetCursorDelta();

        static f32 pitchAngle = 0.0F;
        static f32 yawAngle   = 0.0F;

        pitchAngle += mouseDelta.y * rotSpeed;
        pitchAngle = std::clamp(pitchAngle, -89.0f, 89.0f);
        yawAngle   += mouseDelta.x * rotSpeed;

        cam.transform.rotation = Quaternion::Euler(pitchAngle, 0.0F, 0.0F);
        cam.transform.rotation *= Quaternion::Euler(0.0F, yawAngle, 0.0F);
    }).Run();
}
