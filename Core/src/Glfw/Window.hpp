#ifndef FLK_WINDOW_HPP
#define FLK_WINDOW_HPP

#include "Graphics/Gl.hpp"
#include "Common.hpp"
#include "Event/EventHandler.hpp"
#include "Input/Input.hpp"
#include "Math/Math.hpp"

namespace Flock::Glfw {
    void FLK_API PollEvents();

    bool FLK_API InitGlfw();
    void FLK_API TerminateGlfw();

    class KeyEvent : public Event::Event {
    public:
        i32 key      = 0;
        i32 scancode = 0;
        i32 action   = 0;
        i32 mods     = 0;

        KeyEvent(const i32 key, const i32 scancode, const i32 action, const i32 mods)
            : key(key), scancode(scancode), action(action), mods(mods) {
        }
    };

    class MouseButtonEvent : public Event::Event {
    public:
        i32 button = 0;
        i32 action = 0;
        i32 mods   = 0;

        MouseButtonEvent(const i32 button, const i32 action, const i32 mods)
            : button(button), action(action), mods(mods) {
        }
    };

    class CursorPositionEvent : public Event::Event {
    public:
        f64 xPos = 0.0;
        f64 yPos = 0.0;

        CursorPositionEvent(const f64 xPos, const f64 yPos)
            : xPos(xPos), yPos(yPos) {
        }
    };

    class MouseScrollEvent : public Event::Event {
    public:
        f64 xOffset = 0.0;
        f64 yOffset = 0.0;

        MouseScrollEvent(const f64 xOffset, const f64 yOffset)
            : xOffset(xOffset), yOffset(yOffset) {
        }
    };

    struct FLK_API WindowConfig {
        std::string title           = "Flock";
        Vector2u    size            = {800, 600};
        u32         samplesPerPixel = 4;
        bool        vsync           = true;
    };

    class FLK_API Window {
        inline static u32               s_WindowCount   = 0;
        inline static Window *          s_CurrentWindow = nullptr;
        inline static Event::EventQueue s_EventQueue    = {};

        GLFWwindow * m_GlfwWindowPtr = nullptr;
        WindowConfig m_Config        = {};

    public:
        static std::optional<Window>             Create(const WindowConfig &config = {});
        [[nodiscard]] static OptionalRef<Window> GetCurrentWindow();

        static void PollEvents(Event::EventHandler &eventHandler);

        Window() = default;
        ~Window();

        Window(const Window &other) = delete;
        Window(Window &&other) noexcept;

        Window &operator=(const Window &other) = delete;
        Window &operator=(Window &&other) noexcept;

        void Clear() const;

        [[nodiscard]] std::string GetTitle() const;
        [[nodiscard]] Vector2u    GetSize() const;
        [[nodiscard]] f32         GetAspectRatio() const;
        [[nodiscard]] bool        ShouldClose() const;

        void SetTitle(const std::string &title) const;
        void SetSize(Vector2u size) const;
        void SetCursorMode(Input::CursorMode cursorMode) const;

        void MakeCurrent();
        void SwapBuffers() const;

        bool operator==(const Window &other) const;
        bool operator!=(const Window &other) const;

    private:
        static void KeyCallback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods);
        static void MouseButtonCallback(GLFWwindow *window, i32 button, i32 action, i32 mods);
        static void CursorPosCallback(GLFWwindow *window, f64 xPos, f64 yPos);
        static void MouseScrollCallback(GLFWwindow *window, f64 xOffset, f64 yOffset);

        void SetupCallbacks() const;
    };
}

#endif //FLK_WINDOW_HPP
