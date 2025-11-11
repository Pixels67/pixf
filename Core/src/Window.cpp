#include "Window.hpp"

#include <string>

#include "Common.hpp"
#include "Error/Result.hpp"
#include "Event/Event.hpp"
#include "Graphics/Gl/Gl.hpp"
#include "Debug/Logger.hpp"

constexpr unsigned int GL_VERSION_MAJOR = 3;
constexpr unsigned int GL_VERSION_MINOR = 3;

namespace Pixf::Core {
    Error::Result<Window, WindowError> Window::Create(const WindowConfig &config) {
        if (s_WindowCount == 0) {
            if (const auto err = InitGlfw(config.samplesPerPixel); err != WindowError::None) {
                return err;
            }
        }

        GLFWwindow *window = glfwCreateWindow(static_cast<int>(config.size.x), static_cast<int>(config.size.y),
                                              config.title.c_str(), nullptr, nullptr);

        if (window == nullptr) {
            if (s_WindowCount == 0) {
                TerminateGlfw();
            }

            constexpr auto err = WindowError::GlfwWindowCreationFailed;
            return Error::Result<Window, WindowError>(err);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(config.vsync ? 1 : 0);
        glfwMakeContextCurrent(nullptr);

        s_WindowCount++;

        return Error::Result<Window, WindowError>(Window(window));
    }

    Window::Window(Window &&other) noexcept : m_GlfwWindowPtr(other.m_GlfwWindowPtr) {
        s_WindowCount++;
        other.m_GlfwWindowPtr = nullptr;
    }

    Window &Window::operator=(Window &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_GlfwWindowPtr = other.m_GlfwWindowPtr;
        other.m_GlfwWindowPtr = nullptr;

        return *this;
    }

    Window::~Window() {
        s_WindowCount--;

        Cleanup();

        if (s_WindowCount == 0) {
            PIXF_LOG_INFO("Terminating GLFW");
            TerminateGlfw();
        }
    }

    void Window::SetRenderTarget(Event::EventManager &eventManager) const {
        s_EventManager = &eventManager;
        s_CurrentRenderTarget = m_GlfwWindowPtr;
        glfwMakeContextCurrent(s_CurrentRenderTarget);
        SetupCallbacks();
    }

    void Window::SwapBuffers() {
        if (s_CurrentRenderTarget == nullptr) {
            return;
        }

        glfwSwapBuffers(s_CurrentRenderTarget);
    }

    void Window::PollEvents() { glfwPollEvents(); }

    bool Window::ShouldClose() const { return glfwWindowShouldClose(m_GlfwWindowPtr) != 0; }
    GLFWwindow *Window::GetGlfwWindowPtr() const { return m_GlfwWindowPtr; }

    void Window::SetTitle(const std::string &title) const { glfwSetWindowTitle(m_GlfwWindowPtr, title.c_str()); }

    void Window::Resize(const uvec2 size) const {
        glfwSetWindowSize(m_GlfwWindowPtr, static_cast<int>(size.x), static_cast<int>(size.y));
    }

    std::string Window::GetTitle() const { return glfwGetWindowTitle(m_GlfwWindowPtr); }
    uvec2 Window::GetSize() const {
        int x = 0;
        int y = 0;
        glfwGetWindowSize(m_GlfwWindowPtr, &x, &y);
        return uvec2(x, y);
    }

    void Window::ResetRenderTarget() { s_CurrentRenderTarget = nullptr; }

    Window::Window(GLFWwindow *glfwWindowPtr) : m_GlfwWindowPtr(glfwWindowPtr) {}

    void Window::SetupCallbacks() const {
        glfwSetWindowUserPointer(m_GlfwWindowPtr, s_EventManager);

        // Window size changed
        glfwSetWindowSizeCallback(m_GlfwWindowPtr, [](GLFWwindow *window, const int width, const int height) {
            const auto eventManager = static_cast<Event::EventManager *>(glfwGetWindowUserPointer(window));
            const auto event = WindowSizeChangedEvent(width, height);
            PIXF_LOG_TRACE("Window size changed to: ", width, ", ", height);
            eventManager->QueueEvent<WindowSizeChangedEvent>(event);
        });

        const auto event = WindowSizeChangedEvent(GetSize().x, GetSize().y);
        s_EventManager->QueueEvent<WindowSizeChangedEvent>(event);
    }

    WindowError Window::InitGlfw(const unsigned int sampleCount) {

        if (glfwInit() == 0) {
            constexpr auto err = WindowError::GlfwInitFailed;
            return err;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, static_cast<int>(sampleCount));
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        return WindowError::None;
    }

    void Window::TerminateGlfw() { glfwTerminate(); }

    void Window::Cleanup() const {
        if (s_CurrentRenderTarget == m_GlfwWindowPtr) {
            ResetRenderTarget();
        }

        if (m_GlfwWindowPtr == nullptr) {
            return;
        }

        PIXF_LOG_INFO("Cleaning window");
        glfwSetWindowUserPointer(m_GlfwWindowPtr, nullptr);
        glfwDestroyWindow(m_GlfwWindowPtr);
    }
} // namespace Pixf::Core
