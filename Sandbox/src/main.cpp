#include "Flock.hpp"
#include "Time/Time.hpp"
#include "Audio/AudioSource.hpp"

using namespace Flock;
using namespace Flock::Ecs;
using namespace Flock::Graphics;
using namespace Flock::Input;
using namespace Flock::Audio;

i32 main() {
    f32 rotation = 0.0F;
    App app      = App::Create({
        .windowConfig = {.size = {800, 800}},
    }).value();

    app.AddSystem(Stage::Startup, [](World &world) {
        auto &reg = world.GetRegistry();

        world.GetResource<Camera>().projection = Projection::Perspective;

        const Entity dragon = reg.Create();
        reg.AddComponent(dragon, Transform{.position = {0.0F, -0.6F, 1.2F}});
        reg.AddComponent(dragon, ModelRenderer{.modelPath = "../../../assets/dragon2.ply"});

        const Entity sun = reg.Create();
        reg.AddComponent<Light>(sun, {
                                    .position  = {-1.0F, 1.0F, -1.0F},
                                    .color     = {255, 255, 220},
                                    .intensity = 5.0F,
                                });

        const Entity sky = reg.Create();
        reg.AddComponent<Light>(sky, {
                                    .position  = {1.0F, 1.0F, 1.0F},
                                    .color     = {220, 240, 255},
                                    .intensity = 1.0F,
                                });

        const Entity sound = reg.Create();
        reg.AddComponent<AudioSource>(sound, {
            .audioClipPath = "../../../assets/test.wav",
            .looping = true,
        });

        reg.GetComponent<AudioSource>(sound)->get().Play();
    }).AddSystem(Stage::Update, [&](World &world) {
        const f64  dt    = world.GetResource<Time::TimeState>().deltaTime;
        const auto input = world.GetResource<InputState>();
        auto &     cam   = world.GetResource<Camera>();

        const f32 moveSpeed = 1.0F * dt;
        const f32 rotSpeed  = 60.0F * dt;

        if (input.IsKeyDown(Key::W)) {
            cam.position += Vector3f::Forward() * moveSpeed * cam.rotation;
        }
        if (input.IsKeyDown(Key::S)) {
            cam.position -= Vector3f::Forward() * moveSpeed * cam.rotation;
        }

        if (input.IsKeyDown(Key::D)) {
            cam.position += Vector3f::Right() * moveSpeed * cam.rotation;
        }
        if (input.IsKeyDown(Key::A)) {
            cam.position -= Vector3f::Right() * moveSpeed * cam.rotation;
        }

        if (input.IsKeyDown(Key::LShift)) {
            cam.position += Vector3f::Up() * moveSpeed * cam.rotation;
        }
        if (input.IsKeyDown(Key::LControl)) {
            cam.position -= Vector3f::Up() * moveSpeed * cam.rotation;
        }

        if (input.IsKeyDown(Key::E)) {
            rotation += rotSpeed;
        }
        if (input.IsKeyDown(Key::Q)) {
            rotation -= rotSpeed;
        }

        cam.rotation = Quaternion::Euler(0.0F, rotation, 0.0F);
    }).Run();
}
