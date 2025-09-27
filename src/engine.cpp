#include "engine.h"

//@formatter:off
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// @formatter:on

#include <ext/vector_common.hpp>
#include <iostream>

#include "entity_manager.h"
#include "graphics/graphics.h"
#include "systems_manager.h"
#include "time/time.h"
#include "ui/render_window.h"

namespace engine {
ui::RenderWindow window;
EntityManager manager;
SystemsManager sys_manager;

struct Velocity final : Component {
  ~Velocity() override = default;
  int x = 0;
  int y = 0;
};

struct VelSys final : System {
  ~VelSys() override = default;

  void OnInit(EntityManager& entity_manager) override {
    const Entity entity = entity_manager.CreateEntity();
    entity_manager.AddComponentToEntity(entity, Velocity{});
  }

  void OnUpdate(EntityManager& entity_manager, double delta_time) override {
    for (const auto& entity : entity_manager.GetQuery<Velocity>().entities) {
      entity_manager.GetEntityComponent<Velocity>(entity)->x += 1;
    }

    for (const auto& entity : entity_manager.GetQuery<Velocity>().entities) {
      std::cout << entity_manager.GetEntityComponent<Velocity>(entity)->x
                << '\n';
    }
  }
};

int Initialize(const int window_width, const int window_height,
               const char* window_title) {
  window =
      ui::RenderWindow::CreateWindow(window_title, window_width, window_height);

  glfwSetFramebufferSizeCallback(window.GetWindow(), FramebufferSizeCallback);
  glfwSetCursorPosCallback(window.GetWindow(), MouseCallback);

  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, window_width, window_height);

  sys_manager.AddSystem<VelSys>();
  sys_manager.InitSystems(manager);

  return 0;
}

bool ShouldClose() { return glfwWindowShouldClose(window.GetWindow()) != 0; }

void Update() {
  glfwPollEvents();
  Tick();
  Render();
  sys_manager.UpdateSystems(manager, time::GetDeltaTime());
}

void Tick() {
  time::SetTime(glfwGetTime());
  ProcessInput();
}

graphics::Camera cam{glm::vec3(0, 0, 2), glm::quat(1, 0, 0, 0)};

float pitch = 0.0F;
float yaw = -90.0F;

float last_x = 400;
float last_y = 300;
float prev_mouse_y = 0.0F;

const std::vector CUBE_POSITIONS{
    glm::vec3(0.0F, 0.0F, 0.0F),    glm::vec3(2.0F, 5.0F, -15.0F),
    glm::vec3(-1.5F, -2.2F, -2.5F), glm::vec3(-3.8F, -2.0F, -12.3F),
    glm::vec3(2.4F, -0.4F, -3.5F),  glm::vec3(-1.7F, 3.0F, -7.5F),
    glm::vec3(1.3F, -2.0F, -2.5F),  glm::vec3(1.5F, 2.0F, -2.5F),
    glm::vec3(1.5F, 0.2F, -1.5F),   glm::vec3(-1.3F, 1.0F, -1.5F),
};

void Render() {
  pitch = glm::clamp(pitch, -89.0F, 89.0F);

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cam.LookAt(cam.position + direction, glm::vec3(0, 1, 0));

  glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  constexpr auto texConfig = graphics::TextureConfig{
      graphics::TextureConfig::InterpMode::LINEAR,
      graphics::TextureConfig::WrapMode::MIRRORED_REPEAT};

  auto tex = graphics::Texture("tex.png", texConfig);
  auto shader = graphics::Shader::CreateShader();

  graphics::Material mat{{0.6F, 0.8F, 0.65F, 1.0F}, tex, shader};

  for (unsigned int i = 0; i < 10; i++) {
    auto angle = static_cast<float>(glm::radians(
        (20.0F * static_cast<double>(i)) + (45.0F * glfwGetTime())));
    auto axis = glm::vec3(1.0F, 0.3F, 0.5F);

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
    glfwSetWindowShouldClose(window.GetWindow(), 1);
  }

  const auto cameraSpeed = static_cast<float>(3.0F * time::GetDeltaTime());
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

  constexpr float sensitivity = 0.1F;
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  yaw += x_offset;
  pitch += y_offset;
}
}  // namespace engine
