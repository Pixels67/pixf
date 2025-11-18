#include "Files/Model.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Resources.hpp"
#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Math;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Debug;

int main() {
    std::set_terminate(Error::HandleTerminate);

    const Gl::Window window = Gl::Window::Create({.title = "Pixf"});
    window.MakeCurrent();

    Event::EventManager eventManager;
    window.SetupEvents(eventManager);

    PIXF_GL_CALL(glViewport(0, 0, 800, 600));
    PIXF_GL_CALL(glEnable(GL_DEPTH_TEST));

    Resources resources;
    Renderer renderer;

    Matrix4f transform;
    auto [elements] = Files::LoadModel("backpack.obj", resources);

    float dist;
    float rot;

    while (!window.ShouldClose()) {
        Gl::Window::PollEvents();

        dist += 0.05F;
        rot += 0.1F;

        transform = Matrix4f::Identity();
        transform.ApplyRotation(rot, Vector3f::Up());
        transform.ApplyTranslation(Vector3f(0.0F, 0.0F, dist));

        renderer.BeginPass({.projectionMatrix = Matrix4f::Perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f)});

        for (auto &[mesh, material]: elements) {
            renderer.Submit({.mesh = mesh, .material = material, .modelMatrix = transform});
        }

        renderer.Render(resources);

        window.SwapBuffers();
    }

    return 0;
}
