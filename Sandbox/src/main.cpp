#include "Flock.hpp"

using namespace Flock;
using namespace Flock::Ecs;
using namespace Flock::Graphics;
using namespace Flock::Input;

i32 main() {
    f32 rotation = 0.0F;

    App app;
    app
    .AddSystem(Stage::Startup, [](World &world) {
        auto &reg = world.GetRegistry();

        const Entity e = reg.Create();
        reg.AddComponent(e, Transform{.position = {0.0F, -0.6F, 1.2F}});
        reg.AddComponent(e, ModelRenderer{.modelPath = "../../../assets/dragon1.ply"});
    })
    .AddSystem(Stage::Update, [&](World &world) {
        const auto input = world.GetResource<InputState>();
        auto &     cam   = world.GetResource<Camera>();

        constexpr f32 moveSpeed = 0.02F;
        constexpr f32 rotSpeed  = 1.0F;

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
