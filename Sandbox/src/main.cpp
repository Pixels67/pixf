#include "Flock.hpp"
#include "Graphics/Skybox.hpp"
#include "Graphics/SpriteRenderer.hpp"

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
        assets.SetDefaultPipeline(Asset::PipelineType::Unlit, "../../../assets/unlit.glsl");

        assets.Get<Texture>("../../../assets/Checkerboard.png").Configure({
            .wrapMode = Clamp,
        });

        auto &reg = world.GetRegistry();

        world.GetResource<Camera>().projection         = Projection::Orthographic;
        world.GetResource<Camera>().transform.position = {0.0F, 0.0F, -10.0F};
        world.GetResource<Camera>().size               = 5.0F;

        world.GetResource<AmbientLight>().color = {20, 20, 20};

        reg.Create(
            Transform{
                .position = {0.0F, 0.0F, 0.0F}
            },
            SpriteRenderer{.spritePath = "../../../assets/checkerboard.png"}
        );

        world.GetResource<InputState>().cursorMode = CursorMode::Disabled;
    });

    app.AddSystem(Stage::Update, [&](World &world) {
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
    });

    app.Run();
}
