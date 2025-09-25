#include "render_window.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::UI {
    RenderWindow RenderWindow::CreateWindow(const std::string &title, const unsigned int width, const unsigned int height) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr,
                                              nullptr);
        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window!\n";
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD!\n";
            std::exit(EXIT_FAILURE);
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        const WindowConfig config{title, width, height};

        return {config, window};
    }

    GLFWwindow * RenderWindow::GetWindow() const {
        return m_Window;
    }

    void RenderWindow::Close() const {
        glfwSetWindowShouldClose(m_Window, true);
        glfwDestroyWindow(m_Window);
    }

    RenderWindow::RenderWindow(const WindowConfig &config, GLFWwindow *window)
        : m_Title(config.title), m_Width(config.width), m_Height(config.height), m_Window(window) {
    }
} // namespace Engine::UI
