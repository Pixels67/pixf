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
        world.Load("../../../assets/world.json");

        const auto &assets = world.Resource<Asset::Assets>();

        assets.SetDefaultPipeline(Asset::PipelineType::Pbr, "../../../assets/shader.glsl");
        assets.SetDefaultPipeline(Asset::PipelineType::Unlit, "../../../assets/unlit.glsl");

        world.Resource<Event::EventRegistry>().Add("logPress", [] { Debug::LogInf("Button pressed"); });
        world.Resource<Event::EventRegistry>().Add("logRelease", [] { Debug::LogInf("Button released"); });
    });

    app.AddSystem(Stage::Update, [&](World &world) {
        const f64 dt = world.Resource<Time::TimeState>().deltaTime;

        const auto &input = world.Resource<InputState>();
        auto &      cam   = world.Resource<Camera>();

        const f32 moveSpeed = 5.0F * dt;

        if (input.IsKeyDown(Key::Backspace)) {
            world.Resource<Application>().Close();
        }

        if (input.IsKeyDown(Key::W)) {
            cam.transform.position += Vector3f::Up() * moveSpeed * cam.transform.rotation;
        }
        if (input.IsKeyDown(Key::S)) {
            cam.transform.position -= Vector3f::Up() * moveSpeed * cam.transform.rotation;
        }

        if (input.IsKeyDown(Key::D)) {
            cam.transform.position += Vector3f::Right() * moveSpeed * cam.transform.rotation;
        }
        if (input.IsKeyDown(Key::A)) {
            cam.transform.position -= Vector3f::Right() * moveSpeed * cam.transform.rotation;
        }
    });

    app.Run();
}
