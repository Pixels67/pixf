#include "Application.hpp"

#include "Gui/Gui.hpp"
#include "Input/InputManager.hpp"

namespace Pixf::Core::Application {
    Application::Application(const ApplicationConfig &config) :
        m_Context{.window = Graphics::Gl::Window::Create(config.windowConfig)} {
        m_Context.window.MakeCurrent();

        Input::InputManager::Initialize();
        Gui::Initialize();

        Event::EventManager::Subscribe([&](Event::Event &event) {
            m_Pipeline.OnEvent(m_Context, event);
        });
    }

    void Application::Run() {
        Awake();

        while (!m_Context.window.ShouldClose()) {
            m_Context.clock.StartFrameTimer(glfwGetTime());

            Graphics::Gl::Window::PollEvents();
            Event::EventManager::ProcessEvents();

            const double deltaTime = m_Context.clock.GetDeltaTime();

            Update(deltaTime);
            m_Pipeline.Update(m_Context, deltaTime);

            Render(deltaTime);
            m_Pipeline.Render(m_Context, deltaTime);

            Gui::BeginRenderGui(deltaTime);
            RenderGui(deltaTime);
            m_Pipeline.RenderGui(m_Context, deltaTime);
            Gui::EndRenderGui();

            m_Context.window.SwapBuffers();

            m_Context.clock.EndFrameTimer(glfwGetTime());
        }
    }

    Context &Application::GetContext() { return m_Context; }

    void Application::Awake() {}

    void Application::Update(double deltaTime) {}

    void Application::Render(double deltaTime) {}

    void Application::RenderGui(double deltaTime) {}

    void Application::Shutdown() {}
} // namespace Pixf::Core::Application
