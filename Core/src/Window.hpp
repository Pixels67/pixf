#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include "Common.hpp"
#include "Error/Result.hpp"
#include "Event/Event.hpp"
#include "Graphics/Gl/Gl.hpp"

namespace Pixf::Core {
    constexpr auto g_DefaultWindowSize = uvec2(800, 600);

    struct WindowConfig {
        std::string title;
        uvec2 size = g_DefaultWindowSize;
        uint8_t samplesPerPixel = 4;
        bool vsync = true;
    };

    enum class WindowError : uint8_t {
        None = 0,
        GlfwInitFailed,
        GlfwWindowCreationFailed,
    };

    class WindowSizeChangedEvent final : public Event::Event {
    public:
        unsigned int newWidth = 0;
        unsigned int newHeight = 0;

        WindowSizeChangedEvent() = default;
        WindowSizeChangedEvent(const unsigned int newWidth, const unsigned int newHeight) :
            newWidth(newWidth), newHeight(newHeight) {}
    };

    class Window {
    public:
        PIXF_API static Error::Result<Window, WindowError> Create(const WindowConfig &config);

        Window(const Window &) = delete;
        PIXF_API Window(Window &&other) noexcept;

        Window &operator=(const Window &) = delete;
        PIXF_API Window &operator=(Window &&other) noexcept;

        PIXF_API ~Window();

        PIXF_API void SetRenderTarget(Event::EventManager &eventManager) const;
        PIXF_API static void ResetRenderTarget();

        PIXF_API static void SwapBuffers();
        PIXF_API static void PollEvents();

        PIXF_API bool ShouldClose() const;
        PIXF_API GLFWwindow *GetGlfwWindowPtr() const;

        PIXF_API void SetTitle(const std::string &title) const;
        PIXF_API void Resize(uvec2 size) const;

        PIXF_API std::string GetTitle() const;
        PIXF_API uvec2 GetSize() const;

    private:
        static inline GLFWwindow *s_CurrentRenderTarget = nullptr;
        static inline unsigned int s_WindowCount = 0;
        static inline Event::EventManager *s_EventManager = nullptr;
        GLFWwindow *m_GlfwWindowPtr;

        explicit Window(GLFWwindow *glfwWindowPtr);

        void SetupCallbacks() const;

        static WindowError InitGlfw(unsigned int sampleCount);
        static void TerminateGlfw();

        void Cleanup() const;
    };
} // namespace Pixf::Core

#endif // WINDOW_HPP
