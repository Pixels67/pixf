#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../../Common.hpp"
#include "Error/Result.hpp"
#include "Gl.hpp"

#include <string>

namespace Pixf::Core::Graphics::Gl {
    struct WindowConfig {
        std::string title;
        uvec2 size = vec2(800U, 600U);
        uint8_t samplesPerPixel = 4;
        bool vsync = true;
    };

    enum class WindowError : uint8_t {
        None = 0,
        GlfwInitFailed,
        GlfwWindowCreationFailed,
    };

    class Window {
    public:
        static Error::Result<Window, WindowError> Create(const WindowConfig &config);

        Window(const Window &) = delete;
        Window(Window &&other) noexcept;

        Window &operator=(const Window &) = delete;
        Window &operator=(Window &&other) noexcept;

        ~Window();

        void SetRenderTarget() const;
        static void ResetRenderTarget();

        static void SwapBuffers();
        static void PollEvents();

        bool ShouldClose() const;
        GLFWwindow *GetGlfwWindowPtr() const;

        void SetTitle(const std::string &title) const;
        void Resize(uvec2 size) const;

        std::string GetTitle() const;
        uvec2 GetSize() const;

    private:
        static inline GLFWwindow *s_CurrentRenderTarget = nullptr;
        static inline unsigned int s_WindowCount = 0;
        GLFWwindow *m_GlfwWindowPtr;

        explicit Window(GLFWwindow *glfwWindowPtr);

        static WindowError InitGlfw(unsigned int sampleCount);
        static void TerminateGlfw();

        void Cleanup() const;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // WINDOW_HPP
