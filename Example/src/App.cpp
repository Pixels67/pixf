#include "Error/Error.hpp"
#include "Graphics/Gl/Window.hpp"
#include "Pixf.hpp"

int main() {
    using namespace Pixf::Core::Debug;
    std::set_terminate(Pixf::Core::Error::HandleTerminate);

    const Pixf::Core::Graphics::Gl::Window window = Pixf::Core::Graphics::Gl::Window::Create({.title = "Pixf"});
    window.MakeCurrent();

    Pixf::Core::Event::EventManager eventManager;

    window.SetupEvents(eventManager);

    PIXF_GL_CALL(glViewport(0, 0, 800, 600));

    while (!window.ShouldClose()) {
        Pixf::Core::Graphics::Gl::Window::PollEvents();

        PIXF_GL_CALL(glClearColor(0.12F, 0.12F, 0.12F, 1.0F));
        PIXF_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        window.SwapBuffers();
    }

    return 0;
}
