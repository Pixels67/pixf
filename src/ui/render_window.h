#pragma once

#include <string>

struct GLFWwindow;

namespace Engine::UI {
    struct WindowConfig {
        std::string title;
        unsigned int width;
        unsigned int height;
    };

    class RenderWindow {
    public:
        static RenderWindow CreateWindow(const std::string &title, unsigned int width, unsigned int height);

        RenderWindow() = default;


        [[nodiscard]] GLFWwindow *GetWindow() const;

        void Close() const;
    private:
        std::string m_Title;
        unsigned int m_Width = 0;
        unsigned int m_Height = 0;
        GLFWwindow* m_Window = nullptr;

        RenderWindow(const WindowConfig &config, GLFWwindow* window);
    };
} // namespace Engine::UI