#ifndef FLK_WINDOW_HPP
#define FLK_WINDOW_HPP

#include "Graphics/Gl.hpp"
#include "Common.hpp"
#include "Math/Math.hpp"

namespace Flock::Glfw {
    void FLK_API PollEvents();

    bool FLK_API InitGlfw();
    void FLK_API TerminateGlfw();

    struct FLK_API WindowConfig {
        std::string title           = "Flock";
        Vector2u    size            = {800, 600};
        u32         samplesPerPixel = 4;
        bool        vsync           = true;
    };

    class FLK_API Window {
        inline static u32 s_WindowCount = 0;

        GLFWwindow * m_GlfwWindowPtr = nullptr;
        WindowConfig m_Config        = {};

    public:
        static std::optional<Window> Create(const WindowConfig &config = {});

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

        void MakeCurrent() const;
        void SwapBuffers() const;

    private:
        Window() = default;
    };
}

#endif //FLK_WINDOW_HPP
