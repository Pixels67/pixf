#include "Flock.hpp"
#include "Event/EventRegistry.hpp"
#include "Graphics/Skybox.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Gui/Button.hpp"
#include "Gui/Image.hpp"

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

        const auto &assets = world.GetResource<Asset::Assets>();

        assets.SetDefaultPipeline(Asset::PipelineType::Pbr, "../../../assets/shader.glsl");
        assets.SetDefaultPipeline(Asset::PipelineType::Unlit, "../../../assets/unlit.glsl");

        world.GetResource<Event::EventRegistry>().Add("logPress", [] { Debug::LogInf("Button pressed"); });
        world.GetResource<Event::EventRegistry>().Add("logRelease", [] { Debug::LogInf("Button released"); });
    });

    app.AddSystem(Stage::Update, [&](World &world) {
        const f64 dt = world.GetResource<Time::TimeState>().deltaTime;

        auto &input = world.GetResource<InputState>();
        auto &cam   = world.GetResource<Camera>();

        const f32 moveSpeed = 5.0F * dt;

        if (input.IsKeyDown(Key::Backspace)) {
            world.GetResource<Application>().Close();
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
