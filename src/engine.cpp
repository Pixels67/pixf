#include "engine.h"

#include <iostream>
#include <ext/vector_common.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/graphics.h"
#include "time/time.h"
#include "ui/render_window.h"

namespace Engine {
    UI::RenderWindow Window;

    int Initialize(const int windowWidth, const int windowHeight, const char *windowTitle) {
        Window = UI::RenderWindow::CreateWindow(windowTitle, windowWidth, windowHeight);

        glfwSetFramebufferSizeCallback(Window.GetWindow(), FramebufferSizeCallback);
        glfwSetCursorPosCallback(Window.GetWindow(), MouseCallback);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, windowWidth, windowHeight);
        return 0;
    }

    bool ShouldClose() {
        return glfwWindowShouldClose(Window.GetWindow());
    }

    void Update() {
        glfwPollEvents();
        Tick();
        Render();
    }

    void Tick() {
        Time::SetTime(glfwGetTime());
        ProcessInput();
    }

    Graphics::Camera Cam{glm::vec3(0, 0, 2), glm::quat(1, 0, 0, 0)};

    float Pitch = 0.0f;
    float Yaw   = -90.0f;

    float LastX      = 400;
    float LastY      = 300;
    float PrevMouseY = 0.0f;

    //@formatter:off
    const std::vector CUBE_POSITIONS{
        glm::vec3( 0.0f,  0.0f,   0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,  -7.5f),
        glm::vec3( 1.3f, -2.0f,  -2.5f),
        glm::vec3( 1.5f,  2.0f,  -2.5f),
        glm::vec3( 1.5f,  0.2f,  -1.5f),
        glm::vec3(-1.3f,  1.0f,  -1.5f),
    };
    //@formatter:on

    void Render() {
        Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

        glm::vec3 direction;
        direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        direction.y = sin(glm::radians(Pitch));
        direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Cam.LookAt(Cam.position + direction, glm::vec3(0, 1, 0));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        constexpr auto texConfig = Graphics::TextureConfig{
            Graphics::TextureConfig::InterpMode::Linear,
            Graphics::TextureConfig::WrapMode::MirroredRepeat
        };

        auto tex    = Graphics::Texture("tex.png", texConfig);
        auto shader = Graphics::Shader::CreateShader();

        Graphics::Material mat{{0.6f, 0.8f, 0.65f, 1.0f}, tex, shader};

        for (unsigned int i = 0; i < 10; i++) {
            auto angle = static_cast<float>(glm::radians(20.0f * static_cast<double>(i) + 45.0f * glfwGetTime()));
            auto axis  = glm::vec3(1.0f, 0.3f, 0.5f);

            Core::Transform trans{
                CUBE_POSITIONS[i],
                angleAxis(angle, axis),
                glm::vec3(1, 1, 1)
            };

            Graphics::Mesh(Graphics::CUBE_VERTS).Render(mat, Cam, trans);
        }

        glfwSwapBuffers(Window.GetWindow());
    }

    void Terminate() {
        Window.Close();
    }

    void ProcessInput() {
        if (glfwGetKey(Window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(Window.GetWindow(), true);
        }

        const auto cameraSpeed = static_cast<float>(3.0f * Time::GetDeltaTime());
        if (glfwGetKey(Window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(0, 0, -1)));
        if (glfwGetKey(Window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(0, 0, 1)));
        if (glfwGetKey(Window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(-1, 0, 0)));
        if (glfwGetKey(Window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(1, 0, 0)));
    }

    void FramebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void MouseCallback(GLFWwindow *window, const double xPos, const double yPos) {
        auto xOffset = static_cast<float>(xPos - LastX);
        auto yOffset = static_cast<float>(LastY - yPos);
        LastX        = static_cast<float>(xPos);
        LastY        = static_cast<float>(yPos);

        constexpr float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        Yaw += xOffset;
        Pitch += yOffset;
    }
} // namespace Engine
