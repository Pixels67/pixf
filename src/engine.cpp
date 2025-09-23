#include "engine.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/graphics.h"

namespace Engine {
    GLFWwindow *Window;

    int Initialize(const int windowWidth, const int windowHeight, const char *windowTitle) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
        if (Window == nullptr) {
            std::cerr << "Failed to create GLFW window!\n";
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(Window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD!\n";
            return -1;
        }

        glViewport(0, 0, windowWidth, windowHeight);
        glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
        return 0;
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(Window);
    }

    void Update() {
        glfwPollEvents();
        Tick();
        Render();
    }

    void Tick() {
        ProcessInput();
    }

    void Render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // @formatter:off
        const std::vector vertices{
            // Pos                // UVs
            -1.0f, -1.0f,  0.0f,   0.0f, 0.0f,
            -1.0f,  1.0f,  0.0f,   0.0f, 1.0f,
             1.0f, -1.0f,  0.0f,   1.0f, 0.0f,
             1.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        };

        const std::vector indices{
            0u, 1u, 2u,
            1u, 2u, 3u,
        };
        // @formatter:on

        constexpr auto texConfig = Graphics::TextureConfig{
            Graphics::TextureConfig::InterpMode::Linear,
            Graphics::TextureConfig::WrapMode::Repeat
        };

        const Graphics::Texture tex("tex.jpg", texConfig);
        tex.Bind(0);
        const Graphics::Shader shader = Graphics::Shader::CreateShader();
        const Graphics::RenderObject obj1(vertices, indices);
        obj1.Render(shader);

        glfwSwapBuffers(Window);
    }

    void Terminate() {
        glfwTerminate();
    }

    void ProcessInput() {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(Window, true);
        }
    }

    void FramebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }
} // namespace Engine
