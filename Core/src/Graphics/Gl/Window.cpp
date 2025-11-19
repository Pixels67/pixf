#include "Window.hpp"

#include "Gl.hpp"
#include "Math/Vector.hpp"

namespace Pixf::Core::Graphics::Gl {
    static constexpr int s_OpenGlVersionMajor = 3;
    static constexpr int s_OpenGlVersionMinor = 3;

    Window Window::Create(const WindowConfig &config) {
        if (s_WindowCount == 0) {
            if (glfwInit() == 0) {
                throw WindowError("Failed to initialize GLFW");
            }

            PIXF_CORE_LOG_DEBUG("Initialized GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, s_OpenGlVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, s_OpenGlVersionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *window = glfwCreateWindow(static_cast<int>(config.size.x),
                                              static_cast<int>(config.size.y),
                                              config.title.c_str(),
                                              nullptr,
                                              nullptr);

        if (window == nullptr) {
            if (s_WindowCount == 0) {
                glfwTerminate();
            }

            throw WindowError("Failed to create window");
        }

        GLFWwindow *currentWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(window);

        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            if (s_WindowCount == 0) {
                glfwTerminate();
                glfwDestroyWindow(window);
            }

            throw WindowError("Failed to initialize GLAD");
        }

        glfwMakeContextCurrent(currentWindow);

        s_WindowCount++;

        PIXF_CORE_LOG_DEBUG("Created window: \"{}\"", config.title);

        return Window(window, config);
    }

    Window::~Window() {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        s_WindowCount--;

        glfwSetWindowUserPointer(m_GlfwWindowPtr, nullptr);
        glfwDestroyWindow(m_GlfwWindowPtr);

        PIXF_CORE_LOG_DEBUG("Destroyed window: \"{}\"", m_Config.title);
        if (s_WindowCount == 0) {
            glfwTerminate();
            PIXF_CORE_LOG_DEBUG("Terminated GLFW");
        }
    }

    void Window::PollEvents() { glfwPollEvents(); }

    Math::Vector2u Window::GetSize() const {
        int width = 0;
        int height = 0;

        glfwGetWindowSize(m_GlfwWindowPtr, &width, &height);
        return Math::Vector2u(width, height);
    }

    void Window::SetSize(const Math::Vector2u size) const {
        glfwSetWindowSize(m_GlfwWindowPtr, static_cast<int>(size.x), static_cast<int>(size.y));
    }

    std::string Window::GetTitle() const { return glfwGetWindowTitle(m_GlfwWindowPtr); }

    void Window::SetTitle(const std::string &title) const { glfwSetWindowTitle(m_GlfwWindowPtr, title.c_str()); }

    bool Window::ShouldClose() const { return glfwWindowShouldClose(m_GlfwWindowPtr) != 0; }

    void Window::Close() const { glfwSetWindowShouldClose(m_GlfwWindowPtr, 1); }

    void Window::MakeCurrent() const { glfwMakeContextCurrent(m_GlfwWindowPtr); }

    void Window::SwapBuffers() const { glfwSwapBuffers(m_GlfwWindowPtr); }

    void Window::SetupEvents(Event::EventManager &eventManager) const {
        glfwSetWindowUserPointer(m_GlfwWindowPtr, &eventManager);

        // Window size changed
        glfwSetWindowSizeCallback(m_GlfwWindowPtr, [](GLFWwindow *window, const int width, const int height) {
            auto *const em = static_cast<Event::EventManager *>(glfwGetWindowUserPointer(window));
            const WindowSizeChangedEvent event({static_cast<unsigned>(width), static_cast<unsigned>(height)});
            PIXF_CORE_LOG_TRACE("Window size changed to: {}, {}", width, height);
            em->QueueEvent<WindowSizeChangedEvent>(event);
        });

        const auto event = WindowSizeChangedEvent{GetSize()};
        eventManager.QueueEvent<WindowSizeChangedEvent>(event);
    }

    Window::Window(GLFWwindow *glfwWindowPtr, const WindowConfig &config) :
        m_GlfwWindowPtr(glfwWindowPtr), m_Config(config) {}
} // namespace Pixf::Core::Graphics::Gl
