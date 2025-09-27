#include "engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ext/vector_common.hpp>
#include <iostream>

#include "entity_manager.h"
#include "graphics/graphics.h"
#include "time/time.h"
#include "ui/render_window.h"

namespace engine {
ui::RenderWindow window;
EntityManager manager;
struct Velocity final : Component {
  ~Velocity() override = default;
  int x = 0;
  int y = 0;
};

int Initialize(const int windowWidth, const int windowHeight,
               const char* windowTitle) {
  window =
      ui::RenderWindow::CreateWindow(windowTitle, windowWidth, windowHeight);

  glfwSetFramebufferSizeCallback(window.GetWindow(), FramebufferSizeCallback);
  glfwSetCursorPosCallback(window.GetWindow(), MouseCallback);

  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, windowWidth, windowHeight);
  const Entity e = manager.CreateEntity();
  manager.AddComponentToEntity(e, Velocity{});
  for (const auto& comp : manager.GetQuery<Velocity>().components) {
    comp->x += 1;
  }

  for (const auto& comp : manager.GetQuery<Velocity>().components) {
    std::cout << comp->x << '\n';
  }

  return 0;
}

bool ShouldClose() { return glfwWindowShouldClose(window.GetWindow()); }

void Update() {
  glfwPollEvents();
  Tick();
  Render();
}

void Tick() {
  time::SetTime(glfwGetTime());
  ProcessInput();
}

graphics::Camera cam{glm::vec3(0, 0, 2), glm::quat(1, 0, 0, 0)};

float pitch = 0.0f;
float yaw = -90.0f;

float last_x = 400;
float last_y = 300;
float prev_mouse_y = 0.0f;

const std::vector CUBE_POSITIONS{
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f),
};

void Render() {
  pitch = glm::clamp(pitch, -89.0f, 89.0f);

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cam.LookAt(cam.position + direction, glm::vec3(0, 1, 0));

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  constexpr auto texConfig = graphics::TextureConfig{
      graphics::TextureConfig::InterpMode::LINEAR,
      graphics::TextureConfig::WrapMode::MIRRORED_REPEAT};

  auto tex = graphics::Texture("tex.png", texConfig);
  auto shader = graphics::Shader::CreateShader();

  graphics::Material mat{{0.6f, 0.8f, 0.65f, 1.0f}, tex, shader};

  for (unsigned int i = 0; i < 10; i++) {
    auto angle = static_cast<float>(
        glm::radians(20.0f * static_cast<double>(i) + 45.0f * glfwGetTime()));
    auto axis = glm::vec3(1.0f, 0.3f, 0.5f);

    core::Transform trans{CUBE_POSITIONS[i], angleAxis(angle, axis),
                          glm::vec3(1, 1, 1)};

    graphics::Mesh(graphics::CUBE_VERTS).Render(mat, cam, trans);
  }

  glfwSwapBuffers(window.GetWindow());
}

void Terminate() {
  window.Close();
  glfwTerminate();
}

void ProcessInput() {
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window.GetWindow(), true);
  }

  const auto cameraSpeed = static_cast<float>(3.0f * time::GetDeltaTime());
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) {
    cam.Translate(cam.rotation * (cameraSpeed * glm::vec3(0, 0, -1)));
  }
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    cam.Translate(cam.rotation * (cameraSpeed * glm::vec3(0, 0, 1)));
  }
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    cam.Translate(cam.rotation * (cameraSpeed * glm::vec3(-1, 0, 0)));
  }
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    cam.Translate(cam.rotation * (cameraSpeed * glm::vec3(1, 0, 0)));
  }
}

void FramebufferSizeCallback(GLFWwindow* window, const int width,
                             const int height) {
  glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, const double x_pos, const double y_pos) {
  auto x_offset = static_cast<float>(x_pos - last_x);
  auto y_offset = static_cast<float>(last_y - y_pos);
  last_x = static_cast<float>(x_pos);
  last_y = static_cast<float>(y_pos);

  constexpr float sensitivity = 0.1f;
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  yaw += x_offset;
  pitch += y_offset;
}
}  // namespace engine
