#ifndef PIXF_WINDOW_HPP
#define PIXF_WINDOW_HPP

#include "Common.hpp"
#include "Error/Error.hpp"
#include "Event/EventManager.hpp"
#include "Gl.hpp"
#include "Math/Vector.hpp"

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
        Math::Vector2u newSize{};

        WindowSizeChangedEvent() = default;
        explicit WindowSizeChangedEvent(const Math::Vector2u newSize) : newSize(newSize) {}
    };

    class PIXF_API Window {
    public:
        static Window Create(const WindowConfig &config = {});

        Window(Window &&) = delete;
        Window(const Window &) = delete;
        Window &operator=(Window &&) = delete;
        Window &operator=(const Window &) = delete;

        ~Window();

        static void PollEvents();

        Math::Vector2u GetSize() const;
        void SetSize(Math::Vector2u size) const;

        std::string GetTitle() const;
        void SetTitle(const std::string &title) const;

        bool ShouldClose() const;
        void Close() const;

        void MakeCurrent() const;
        void SwapBuffers() const;

        void SetupEvents(Event::EventManager &eventManager) const;

    private:
        inline static unsigned int s_WindowCount = 0;
        GLFWwindow *m_GlfwWindowPtr;
        WindowConfig m_Config;

        explicit Window(GLFWwindow *glfwWindowPtr, const WindowConfig &config);
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_WINDOW_HPP
