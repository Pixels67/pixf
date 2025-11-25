#ifndef PIXF_WINDOW_HPP
#define PIXF_WINDOW_HPP

#include "Common.hpp"
#include "Error/Error.hpp"
#include "Event/EventManager.hpp"
#include "Gl.hpp"
#include "Math/Vector.hpp"

namespace Pixf::Core::Input {
    class InputManager;
}

namespace Pixf::Core::Graphics::Gl {
    struct PIXF_API WindowConfig {
        std::string title;
        Math::Vector2u size = {800, 600};
    };

    class PIXF_API WindowError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API WindowSizeChangedEvent final : public Event::Event {
    public:
        Math::Vector2u newSize;

        WindowSizeChangedEvent() = default;
        explicit WindowSizeChangedEvent(const Math::Vector2u newSize) : newSize(newSize) {}
    };

    class PIXF_API KeyEvent final : public Event::Event {
    public:
        int keycode = 0;
        int scancode = 0;
        int action = 0;
        int mods = 0;

        KeyEvent() = default;
        explicit KeyEvent(const int keycode, const int scancode, const int action, const int mods) :
            keycode(keycode), scancode(scancode), action(action), mods(mods) {}
    };

    class PIXF_API MouseKeyEvent final : public Event::Event {
    public:
        int keycode = 0;
        int action = 0;
        int mods = 0;

        MouseKeyEvent() = default;
        explicit MouseKeyEvent(const int keycode, const int action, const int mods) :
            keycode(keycode), action(action), mods(mods) {}
    };

    class PIXF_API MouseMovedEvent final : public Event::Event {
    public:
        Math::Vector2d newPosition{};

        MouseMovedEvent() = default;
        explicit MouseMovedEvent(const Math::Vector2d newPosition) : newPosition(newPosition) {}
    };

    class PIXF_API CharEvent final : public Event::Event {
    public:
        unsigned int codepoint = 0;

        CharEvent() = default;
        explicit CharEvent(const unsigned int codepoint) : codepoint(codepoint) {}
    };

    class PIXF_API Window {
    public:
        static Window Create(const WindowConfig &config = {});
        static std::optional<const Window *> GetCurrent();

        Window(Window &&) = delete;
        Window(const Window &) = delete;
        Window &operator=(Window &&) = delete;
        Window &operator=(const Window &) = delete;

        ~Window();

        static void PollEvents();

        Math::Vector2u GetSize() const;
        void SetSize(Math::Vector2u size) const;

        float GetAspectRatio() const;

        std::string GetTitle() const;
        void SetTitle(const std::string &title) const;

        bool ShouldClose() const;
        void Close() const;

        void MakeCurrent() const;
        void SwapBuffers() const;

        unsigned int GetKey(unsigned int key) const;
        unsigned int GetMouseKey(unsigned int key) const;
        Math::Vector2d GetMousePosition() const;

    private:
        inline static unsigned int s_WindowCount = 0;
        inline static const Window *s_CurrentWindow = nullptr;

        GLFWwindow *m_GlfwWindowPtr;
        WindowConfig m_Config;

        explicit Window(GLFWwindow *glfwWindowPtr, const WindowConfig &config);
        void SetupEvents() const;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_WINDOW_HPP
