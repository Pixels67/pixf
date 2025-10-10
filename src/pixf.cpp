#include "pixf.h"

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>
#include <assimp/mesh.h>
#include <entity_manager.h>

#include <glm.hpp>

#include "graphics/graphics.h"
#include "graphics/model.h"
#include "systems_manager.h"
#include "time/time.h"
#include "ui/render_window.h"

namespace pixf {
ui::RenderWindow window;
EntityManager entity_manager;
SystemsManager sys_manager;

void Initialize(const int window_width, const int window_height, const char* window_title) {
  window = ui::RenderWindow::CreateWindow(window_title, window_width, window_height);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window.GetWindow(), FramebufferSizeCallback);
  glfwSetCursorPosCallback(window.GetWindow(), MouseCallback);
  glEnable(GL_MULTISAMPLE);

  entity_manager.CreateSingleton<graphics::ResourceManager>({});
  auto& resource_manager = *entity_manager.GetSingleton<graphics::ResourceManager>();

  graphics::Camera camera;
  camera.bg_color = glm::vec3(0.15F, 0.15F, 0.15F);
  camera.viewport_size = glm::vec2(window_width, window_height);
  entity_manager.CreateSingleton<graphics::Camera>(camera);

  graphics::lighting::PointLight point_light;
  point_light.position = glm::vec3(4.0F, -2.0F, 8.0F);
  point_light.color = glm::vec3(0.5F, 0.4F, 0.3F);
  point_light.intensity = 1.0F;
  entity_manager.AddComponentToEntity<graphics::lighting::PointLight>(entity_manager.CreateEntity(),
                                                                      point_light);
  point_light.position = glm::vec3(-4.0F, 0.0F, 4.0F);
  point_light.color = glm::vec3(0.0F, 0.1F, 0.2F);
  point_light.intensity = 0.5F;
  entity_manager.AddComponentToEntity<graphics::lighting::PointLight>(entity_manager.CreateEntity(),
                                                                      point_light);

  graphics::ModelRenderer object{};
  object.model = graphics::Model{"backpack.obj", resource_manager};
  object.materials = graphics::Material::LoadFromModel("backpack.obj", resource_manager);

  core::Transform transform;
  transform.position = glm::vec3(0.0F, -1.2F, 4.0F);
  Entity entity = entity_manager.CreateEntity();

  entity_manager.AddComponentToEntity(entity, transform);
  entity_manager.AddComponentToEntity(entity, object);

  sys_manager.AddSystem<graphics::RenderSystem>();
  sys_manager.InitSystems(entity_manager);
}

bool ShouldClose() { return glfwWindowShouldClose(window.GetWindow()) != 0; }

void Update() {
  glfwPollEvents();
  Tick();
  sys_manager.UpdateSystems(entity_manager, time::GetDeltaTime());
  glfwSwapBuffers(window.GetWindow());
}

double i = 0.0;

void Tick() {
  ProcessInput();
  time::SetTime(glfwGetTime());
  if (i >= 0.05) {
    glfwSetWindowTitle(window.GetWindow(),
                       std::to_string(static_cast<int>(1.0F / time::GetDeltaTime())).c_str());
    i = 0.0;
  }

  i += time::GetDeltaTime();
}

void Terminate() {
  window.Close();
  glfwTerminate();
}

void ProcessInput() {
  if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window.GetWindow(), 1);
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) {
    const auto cam = entity_manager.GetSingleton<graphics::Camera>();
    cam->transform.position += 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 0.0F, 1.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    const auto cam = entity_manager.GetSingleton<graphics::Camera>();
    cam->transform.position -= 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 0.0F, 1.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    const auto cam = entity_manager.GetSingleton<graphics::Camera>();
    cam->transform.position += 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(1.0F, 0.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    const auto cam = entity_manager.GetSingleton<graphics::Camera>();
    cam->transform.position -= 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(1.0F, 0.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    const auto cam = entity_manager.GetSingleton<graphics::Camera>();
    cam->transform.position += 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    const auto cam = entity_manager.GetSingleton<graphics::Camera>();
    cam->transform.position -= 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_E) == GLFW_PRESS) {
    entity_manager.GetSingleton<graphics::Camera>()->transform.Rotate(
        100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
    entity_manager.GetSingleton<graphics::Camera>()->transform.Rotate(
        -100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_F) == GLFW_PRESS) {
    entity_manager.GetSingleton<graphics::Camera>()->transform.Rotate(
        100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(1.0F, 0.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
    entity_manager.GetSingleton<graphics::Camera>()->transform.Rotate(
        -100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(1.0F, 0.0F, 0.0F));
  }
}

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
  glViewport(0, 0, width, height);
  if (entity_manager.IsSingletonRegistered<graphics::Camera>()) {
    entity_manager.GetSingleton<graphics::Camera>()->viewport_size = glm::vec2(width, height);
  }
}

void MouseCallback(GLFWwindow* window, const double x_pos, const double y_pos) {}
}  // namespace pixf
