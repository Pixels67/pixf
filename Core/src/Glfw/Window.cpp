#include "Window.hpp"

namespace Flock::Glfw {
    static constexpr i32 s_OpenGlVersionMajor = 3;
    static constexpr i32 s_OpenGlVersionMinor = 3;

    void PollEvents() {
        glfwPollEvents();
    }

    bool InitGlfw() {
        return glfwInit() == GLFW_TRUE;
    }

    void TerminateGlfw() {
        glfwTerminate();
    }

    std::optional<Window> Window::Create(const WindowConfig &config) {
        if (s_WindowCount == 0) {
            if (!InitGlfw()) {
                return std::nullopt;
            }
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, s_OpenGlVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, s_OpenGlVersionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, static_cast<i32>(config.samplesPerPixel));

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *windowPtr = glfwCreateWindow(
            static_cast<i32>(config.size.x),
            static_cast<i32>(config.size.y),
            config.title.c_str(),
            nullptr,
            nullptr
        );

        if (windowPtr == nullptr) {
            if (s_WindowCount == 0) {
                TerminateGlfw();
            }

            return std::nullopt;
        }

        GLFWwindow *currentWindow = glfwGetCurrentContext();

        glfwMakeContextCurrent(windowPtr);
        glfwSwapInterval(config.vsync);

        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            if (s_WindowCount == 0) {
                TerminateGlfw();
                glfwDestroyWindow(windowPtr);
            }

            return std::nullopt;
        }

        glfwMakeContextCurrent(currentWindow);

        s_WindowCount++;

        Window window{};
        window.m_GlfwWindowPtr = windowPtr;
        window.m_Config        = config;

        return window;
    }

    Window::~Window() {
        Clear();
    }

    Window::Window(Window &&other) noexcept {
        m_GlfwWindowPtr       = other.m_GlfwWindowPtr;
        m_Config              = other.m_Config;
        other.m_GlfwWindowPtr = nullptr;
    }

    Window &Window::operator=(Window &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_GlfwWindowPtr       = other.m_GlfwWindowPtr;
        m_Config              = other.m_Config;
        other.m_GlfwWindowPtr = nullptr;

        return *this;
    }

    void Window::Clear() const {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        glfwDestroyWindow(m_GlfwWindowPtr);
        s_WindowCount--;

        if (s_WindowCount == 0) {
            TerminateGlfw();
        }
    }

    std::string Window::GetTitle() const {
        return glfwGetWindowTitle(m_GlfwWindowPtr);
    }

    Vector2u Window::GetSize() const {
        i32 width;
        i32 height;
        glfwGetWindowSize(m_GlfwWindowPtr, &width, &height);

        return Vector2u{static_cast<u32>(width), static_cast<u32>(height)};
    }

    f32 Window::GetAspectRatio() const {
        const Vector2f size = GetSize();
        return size.x / size.y;
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_GlfwWindowPtr) == GLFW_TRUE;
    }

    void Window::SetTitle(const std::string &title) const {
        glfwSetWindowTitle(m_GlfwWindowPtr, title.c_str());
    }

    void Window::SetSize(Vector2u size) const {
        glfwSetWindowSize(m_GlfwWindowPtr, size.x, size.y);
    }

    void Window::MakeCurrent() const {
        glfwMakeContextCurrent(m_GlfwWindowPtr);
    }

    void Window::SwapBuffers() const {
        glfwSwapBuffers(m_GlfwWindowPtr);
    }
}
