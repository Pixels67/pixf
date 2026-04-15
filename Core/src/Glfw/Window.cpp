#include "Window.hpp"

#include "Input/Input.hpp"
#include "glad/glad.h"

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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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
        window.SetupCallbacks();

        return window;
    }

    OptionalRef<Window> Window::CurrentWindow() {
        if (s_CurrentWindow) {
            return *s_CurrentWindow;
        }

        return std::nullopt;
    }

    void Window::PollEvents(Event::EventHandler &eventHandler) {
        Glfw::PollEvents();
        while (!s_EventQueue.IsEmpty()) {
            eventHandler.QueueEvent(s_EventQueue.PopEvent());
        }
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

    std::string Window::Title() const {
        if (m_GlfwWindowPtr == nullptr) {
            return "";
        }

        return glfwGetWindowTitle(m_GlfwWindowPtr);
    }

    Vector2u Window::Size() const {
        if (m_GlfwWindowPtr == nullptr) {
            return {};
        }

        i32 width;
        i32 height;
        glfwGetWindowSize(m_GlfwWindowPtr, &width, &height);

        return Vector2u{static_cast<u32>(width), static_cast<u32>(height)};
    }

    f32 Window::AspectRatio() const {
        if (m_GlfwWindowPtr == nullptr) {
            return 0.0F;
        }

        const Vector2f size = Size();
        return size.x / size.y;
    }

    bool Window::ShouldClose() const {
        if (m_GlfwWindowPtr == nullptr) {
            return true;
        }

        return glfwWindowShouldClose(m_GlfwWindowPtr) == GLFW_TRUE;
    }

    void Window::SetTitle(const std::string &title) const {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        glfwSetWindowTitle(m_GlfwWindowPtr, title.c_str());
    }

    void Window::SetSize(const Vector2u size) const {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        glfwSetWindowSize(m_GlfwWindowPtr, size.x, size.y);
    }

    void Window::SetCursorMode(const Input::CursorMode cursorMode) const {
        glfwSetInputMode(m_GlfwWindowPtr, GLFW_CURSOR, Input::ToGlfwType(cursorMode));
    }

    void Window::MakeCurrent() {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        s_CurrentWindow = this;
        glfwMakeContextCurrent(m_GlfwWindowPtr);
    }

    void Window::SwapBuffers() const {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        glfwSwapBuffers(m_GlfwWindowPtr);
    }

    bool Window::operator==(const Window &other) const {
        return m_GlfwWindowPtr == other.m_GlfwWindowPtr;
    }

    bool Window::operator!=(const Window &other) const {
        return m_GlfwWindowPtr != other.m_GlfwWindowPtr;
    }

    void Window::KeyCallback(GLFWwindow *window, const i32 key, const i32 scancode, const i32 action, const i32 mods) {
        if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            return;
        }

        s_EventQueue.QueueEvent(KeyEvent{key, scancode, action, mods});
    }

    void Window::MouseButtonCallback(GLFWwindow *window, const i32 button, const i32 action, const i32 mods) {
        if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            return;
        }

        s_EventQueue.QueueEvent(MouseButtonEvent{button, action, mods});
    }

    void Window::CursorPosCallback(GLFWwindow *window, const f64 xPos, const f64 yPos) {
        if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            return;
        }

        s_EventQueue.QueueEvent(CursorPositionEvent{xPos, yPos});
    }

    void Window::MouseScrollCallback(GLFWwindow *window, const f64 xOffset, const f64 yOffset) {
        if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            return;
        }

        s_EventQueue.QueueEvent(MouseScrollEvent{xOffset, yOffset});
    }

    void Window::SetupCallbacks() const {
        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        glfwSetKeyCallback(m_GlfwWindowPtr, KeyCallback);
        glfwSetMouseButtonCallback(m_GlfwWindowPtr, MouseButtonCallback);
        glfwSetCursorPosCallback(m_GlfwWindowPtr, CursorPosCallback);
        glfwSetScrollCallback(m_GlfwWindowPtr, MouseScrollCallback);
    }
}
