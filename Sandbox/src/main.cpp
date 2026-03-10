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
        world.GetResource<Camera>().transform.position = {0.0F, -1.0F, -10.0F};

        const Entity sphere = reg.Create();
        reg.AddComponent(sphere, Transform{.position = {0.0F, 3.0F, 0.0F}});
        reg.AddComponent(sphere, ModelRenderer{.modelPath = "../../../assets/sphere.glb"});
        reg.AddComponent(sphere, Physics::SphereCollider{});
        reg.AddComponent(sphere, Physics::RigidBody{.mass = 1.0F});

        const Entity ground = reg.Create();
        reg.AddComponent(ground, Transform{.position = {0.0F, -3.0F, 0.0F}, .scale = {10.0F, 0.5F, 10.0F}});
        reg.AddComponent(ground, ModelRenderer{.modelPath = "../../../assets/cube.glb"});
        reg.AddComponent(ground, Physics::BoxCollider{});
        reg.AddComponent(ground, Physics::RigidBody{.mode = Physics::SimulationMode::Static});

        const Entity sun = reg.Create();
        reg.AddComponent<Light>(sun, {
                                    .position  = {-5.0F, 5.0F, -5.0F},
                                    .color     = {255, 255, 220},
                                    .intensity = 5.0F,
                                });

        const Entity sky = reg.Create();
        reg.AddComponent<Light>(sky, {
                                    .position  = {5.0F, 5.0F, 5.0F},
                                    .color     = {220, 240, 255},
                                    .intensity = 1.0F,
                                });
    }).AddSystem(Stage::Update, [&](World &world) {
        const f64  dt    = world.GetResource<Time::TimeState>().deltaTime;
        const auto input = world.GetResource<InputState>();
        auto &     cam   = world.GetResource<Camera>();

        const f32 moveSpeed = 5.0F * dt;
        const f32 rotSpeed  = 60.0F * dt;

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

        if (input.IsKeyDown(Key::E)) {
            rotation += rotSpeed;
        }
        if (input.IsKeyDown(Key::Q)) {
            rotation -= rotSpeed;
        }

        cam.transform.rotation = Quaternion::Euler(0.0F, rotation, 0.0F);
    }).AddSystem(Stage::Update, [&](World &world) {
        const f64  dt    = world.GetResource<Time::TimeState>().deltaTime;
        const auto input = world.GetResource<InputState>();

        const f32 moveSpeed = 5.0F * dt;

        world.GetRegistry().ForEach<Physics::RigidBody>([&](Physics::RigidBody &rb) {
            if (input.IsKeyDown(Key::Up)) {
                rb.linearVelocity.z += moveSpeed;
            }
            if (input.IsKeyDown(Key::Down)) {
                rb.linearVelocity.z -= moveSpeed;
            }
            if (input.IsKeyDown(Key::Right)) {
                rb.linearVelocity.x += moveSpeed;
            }
            if (input.IsKeyDown(Key::Left)) {
                rb.linearVelocity.x -= moveSpeed;
            }
        });
    }).Run();
}
