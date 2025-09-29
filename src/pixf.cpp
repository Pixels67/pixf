#include "pixf.h"

#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>

#include <iostream>

#include "entity_manager.h"
#include "graphics/graphics.h"
#include "systems_manager.h"
#include "time/time.h"
#include "ui/render_window.h"

namespace pixf {
ui::RenderWindow window;
EntityManager manager;
SystemsManager sys_manager;

void Initialize(const int window_width, const int window_height, const char* window_title) {
  window = ui::RenderWindow::CreateWindow(window_title, window_width, window_height);

  glfwSetFramebufferSizeCallback(window.GetWindow(), FramebufferSizeCallback);
  glfwSetCursorPosCallback(window.GetWindow(), MouseCallback);

  graphics::PerspectiveCamera camera;
  camera.viewport_size = glm::vec2(800, 600);

  manager.AddComponentToEntity(manager.CreateEntity(), camera);

  graphics::Renderable renderable;
  renderable.mesh = graphics::Mesh(graphics::CUBE_VERTS);
  // renderable.material.SetTexture(graphics::Texture("tex.png"));

  core::Transform transform;
  transform.position = glm::vec3(-2.0F, 0.0F, -4.0F);
  transform.rotation =
      glm::quat(glm::vec3(glm::radians(45.0F), glm::radians(45.0F), glm::radians(45.0F)));

  for (int i = 0; i < 3; i++) {
    Entity entity = manager.CreateEntity();

    manager.AddComponentToEntity(entity, transform);
    manager.AddComponentToEntity(entity, renderable);
    transform.position.x += 2.0F;
  }

  sys_manager.AddSystem<graphics::RenderSystem>();
  sys_manager.InitSystems(manager);
}

bool ShouldClose() { return glfwWindowShouldClose(window.GetWindow()) != 0; }

void Update() {
  glfwPollEvents();
  Tick();
  sys_manager.UpdateSystems(manager, time::GetDeltaTime());
  glfwSwapBuffers(window.GetWindow());
}

void Tick() {
  time::SetTime(glfwGetTime());
  ProcessInput();
}

void Terminate() {
  window.Close();
  glfwTerminate();
}

void ProcessInput() {
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window.GetWindow(), 1);
  }
}

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
  glViewport(0, 0, width, height);
  for (auto& [entity, camera] : manager.Query<graphics::OrthographicCamera>()) {
    camera->viewport_size = glm::vec2(width, height);
  }

  for (auto& [entity, camera] : manager.Query<graphics::PerspectiveCamera>()) {
    camera->viewport_size = glm::vec2(width, height);
  }
}

void MouseCallback(GLFWwindow* window, const double x_pos, const double y_pos) {}
}  // namespace pixf
