#include "engine.h"

#include <iostream>
#include <matrix.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/vector_common.hpp"
#include "graphics/graphics.h"
#include "time/time.h"

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

        glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
        glfwSetCursorPosCallback(Window, MouseCallback);
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, windowWidth, windowHeight);
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
        Time::SetTime(glfwGetTime());
        ProcessInput();
    }

    Graphics::Camera Cam{glm::vec3(0, 0, 2), glm::quat(1, 0, 0, 0)};

    float Pitch = 0.0f;
    float Yaw   = -90.0f;

    float LastX      = 400, LastY = 300;
    float PrevMouseY = 0.0f;

    // @formatter:off
    const std::vector VERTICES{
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},

        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        Vertex{{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},

        Vertex{{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

        Vertex{{0.5f,  0.5f,  0.5f},   {1.0f, 0.0f}},
        Vertex{{0.5f,  0.5f, -0.5f},   {1.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, -0.5f},   {0.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, -0.5f},   {0.0f, 1.0f}},
        Vertex{{0.5f, -0.5f,  0.5f},   {0.0f, 0.0f}},
        Vertex{{0.5f,  0.5f,  0.5f},   {1.0f, 0.0f}},

        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{ 0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

        Vertex{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
    };

    const std::vector INDICES{
         0u,  1u,  2u,
         3u,  4u,  5u,
         6u,  7u,  8u,
         9u, 10u, 11u,
        12u, 13u, 14u,
        15u, 16u, 17u,
        18u, 19u, 20u,
        21u, 22u, 23u,
        24u, 25u, 26u,
        27u, 28u, 29u,
        30u, 31u, 32u,
        33u, 34u, 35u,
    };

    constexpr glm::vec3 CUBE_POSITIONS[] = {
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

        const Graphics::Mesh obj(VERTICES, INDICES);

        auto tex = Graphics::Texture("tex.png", texConfig);
        auto shader = Graphics::Shader::CreateShader();

        Graphics::Material mat{{0.6f, 0.8f, 0.6f, 1.0f}, tex, shader};

        for (unsigned int i = 0; i < 10; i++) {
            float angle = glm::radians(20.0f * i + 45.0f * glfwGetTime());
            auto axis   = glm::vec3(1.0f, 0.3f, 0.5f);

            obj.Render(mat, {CUBE_POSITIONS[i], angleAxis(angle, axis), glm::vec3(1, 1, 1)}, Cam);
        }

        glfwSwapBuffers(Window);
    }

    void Terminate() {
        glfwTerminate();
    }

    void ProcessInput() {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(Window, true);
        }

        const float cameraSpeed = 3.0f * Time::GetDeltaTime();
        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(0, 0, -1)));
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(0, 0, 1)));
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(-1, 0, 0)));;
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
            Cam.Translate(Cam.rotation * (cameraSpeed * glm::vec3(1, 0, 0)));
    }

    void FramebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void MouseCallback(GLFWwindow *window, const double xPos, const double yPos) {
        float xOffset = xPos - LastX;
        float yOffset = LastY - yPos;
        LastX         = xPos;
        LastY         = yPos;

        constexpr float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        Yaw += xOffset;
        Pitch += yOffset;
    }
} // namespace Engine
