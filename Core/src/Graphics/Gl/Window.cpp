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

    std::optional<const Window *> Window::GetCurrent() {
        if (s_CurrentWindow != nullptr) {
            return s_CurrentWindow;
        }

        return std::nullopt;
    }

    Window::~Window() {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        s_WindowCount--;

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

    void Window::MakeCurrent() const {
        s_CurrentWindow = this;
        glfwMakeContextCurrent(m_GlfwWindowPtr);
    }

    void Window::SwapBuffers() const { glfwSwapBuffers(m_GlfwWindowPtr); }

    unsigned int Window::GetKey(const unsigned int key) const { return glfwGetKey(m_GlfwWindowPtr, key); }

    unsigned int Window::GetMouseKey(const unsigned int key) const { return glfwGetMouseButton(m_GlfwWindowPtr, key); }

    Math::Vector2d Window::GetMousePosition() const {
        double x = 0;
        double y = 0;
        glfwGetCursorPos(m_GlfwWindowPtr, &x, &y);

        return Math::Vector2d(x, y);
    }

    Window::Window(GLFWwindow *glfwWindowPtr, const WindowConfig &config) :
        m_GlfwWindowPtr(glfwWindowPtr), m_Config(config) {
        SetupEvents();
    }

    void Window::SetupEvents() const {
        // Window size changed event
        glfwSetWindowSizeCallback(m_GlfwWindowPtr, [](GLFWwindow *window, int width, int height) {
            const WindowSizeChangedEvent event({static_cast<unsigned>(width), static_cast<unsigned>(height)});
            PIXF_CORE_LOG_TRACE("Window size changed to: {}, {}", width, height);
            Event::EventManager::QueueEvent(event);
        });

        // Key event
        glfwSetKeyCallback(m_GlfwWindowPtr, [](GLFWwindow *window, int keycode, int scancode, int action, int mods) {
            const KeyEvent event(keycode, scancode, action, mods);
            Event::EventManager::QueueEvent(event);
        });

        // Mouse key event
        glfwSetMouseButtonCallback(m_GlfwWindowPtr, [](GLFWwindow *window, int keycode, int action, int mods) {
            const MouseKeyEvent event(keycode, action, mods);
            Event::EventManager::QueueEvent(event);
        });

        // Mouse moved event
        glfwSetCursorPosCallback(m_GlfwWindowPtr, [](GLFWwindow *window, double xPos, double yPos) {
            const MouseMovedEvent event(Math::Vector2d(xPos, yPos));
            Event::EventManager::QueueEvent(event);
        });

        // Char event
        glfwSetCharCallback(m_GlfwWindowPtr, [](GLFWwindow *window, unsigned int codepoint) {
            const CharEvent event(codepoint);
            Event::EventManager::QueueEvent(event);
        });

        const auto event = WindowSizeChangedEvent{GetSize()};
        Event::EventManager::QueueEvent<WindowSizeChangedEvent>(event);
    }
} // namespace Pixf::Core::Graphics::Gl
