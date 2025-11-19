#include "Application.hpp"

namespace Pixf::Core::Application {
    Application::Application(const ApplicationConfig &config) :
        m_State{.window = Graphics::Gl::Window::Create(config.windowConfig)} {
        m_State.window.MakeCurrent();
        m_State.window.SetupEvents(m_State.eventManager);

        m_State.eventManager.Subscribe([&](Event::Event &event) {
            m_Pipeline.OnEvent(m_State, event);
        });
    }

    void Application::Run() {
        Awake();

        while (!m_State.window.ShouldClose()) {
            m_State.clock.StartFrameTimer(glfwGetTime());
            Graphics::Gl::Window::PollEvents();

            const double deltaTime = m_State.clock.GetDeltaTime();

            Update(deltaTime);
            m_Pipeline.Update(m_State, deltaTime);

            Render(deltaTime);
            m_Pipeline.Render(m_State, deltaTime);

            RenderGui(deltaTime);
            m_Pipeline.RenderGui(m_State, deltaTime);

            m_State.window.SwapBuffers();
            m_State.clock.EndFrameTimer(glfwGetTime());
        }
    }

    State &Application::GetState() { return m_State; }

    void Application::Awake() {}

    void Application::Update(double deltaTime) {}

    void Application::Render(double deltaTime) {}

    void Application::RenderGui(double deltaTime) {}

    void Application::Shutdown() {}
} // namespace Pixf::Core::Application
