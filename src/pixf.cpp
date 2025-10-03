#include "pixf.h"

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>

#include <glm.hpp>

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
  glfwSwapInterval(0);

  glfwSetFramebufferSizeCallback(window.GetWindow(), FramebufferSizeCallback);
  glfwSetCursorPosCallback(window.GetWindow(), MouseCallback);

  manager.CreateSingleton<graphics::ResourceManager>({});

  graphics::PerspectiveCamera camera;
  camera.bg_color = glm::vec3(0.12F, 0.15F, 0.15F);
  camera.viewport_size = glm::vec2(window_width, window_height);
  manager.CreateSingleton<graphics::PerspectiveCamera>(camera);

  graphics::gl::lighting::PointLight point_light;
  point_light.position = glm::vec3(-4.0F, 0.0F, 3.0F);
  point_light.color = glm::vec3(1.0F, 0.5F, 0.0F);
  point_light.intensity = 0.5f;
  manager.AddComponentToEntity<graphics::gl::lighting::PointLight>(manager.CreateEntity(),
                                                                   point_light);
  point_light.position = glm::vec3(4.0F, 0.0F, 3.0F);
  point_light.color = glm::vec3(0.0F, 0.5F, 1.0F);
  manager.AddComponentToEntity<graphics::gl::lighting::PointLight>(manager.CreateEntity(),
                                                                   point_light);

  point_light.position = glm::vec3(0.0F, 0.0F, 10.0F);
  point_light.color = glm::vec3(0.25F, 1.0F, 0.25F);
  manager.AddComponentToEntity<graphics::gl::lighting::PointLight>(manager.CreateEntity(),
                                                                   point_light);

  graphics::Renderable renderable;
  renderable.mesh =
      manager.GetSingleton<graphics::ResourceManager>()->CreateMesh(graphics::CUBE_VERTS);
  graphics::gl::TextureConfig config{
      graphics::gl::TextureConfig::InterpMode::NEAREST,
      graphics::gl::TextureConfig::WrapMode::CLAMP_TO_EDGE,
  };

  renderable.material.diffuse_map =
      manager.GetSingleton<graphics::ResourceManager>()->CreateTexture("tex.png", config);
  renderable.material.metallic_map =
      manager.GetSingleton<graphics::ResourceManager>()->CreateTexture("metallic.png", config);
  renderable.material.roughness_map =
      manager.GetSingleton<graphics::ResourceManager>()->CreateTexture("roughness.png", config);

  graphics::ShaderHandle shader = manager.GetSingleton<graphics::ResourceManager>()->CreateShader();
  renderable.material.shader = shader;

  core::Transform transform;
  transform.position = glm::vec3(-2.0F, 0.0F, 4.0F);
  transform.rotation =
      glm::quat(glm::vec3(glm::radians(45.0F), glm::radians(0.0F), glm::radians(45.0F)));

  for (int k = 0; k < 3; k++)
    for (int i = 0; i < 3; i++) {
      Entity entity = manager.CreateEntity();

      transform.position.x = 2.0F * (i - 1);
      transform.position.z = 2.0F * (k + 2);
      transform.position.y = 1.0F * (k - 1);

      manager.AddComponentToEntity(entity, transform);
      manager.AddComponentToEntity(entity, renderable);
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

float i = 0.0F;

void Tick() {
  time::SetTime(glfwGetTime());
  ProcessInput();
  if (i >= 0.05F) {
    glfwSetWindowTitle(window.GetWindow(),
                       std::to_string(static_cast<int>(1.0F / time::GetDeltaTime())).c_str());
    i = 0.0F;
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
    const auto cam = manager.GetSingleton<graphics::PerspectiveCamera>();
    cam->transform.position += 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 0.0F, 1.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    const auto cam = manager.GetSingleton<graphics::PerspectiveCamera>();
    cam->transform.position -= 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 0.0F, 1.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    const auto cam = manager.GetSingleton<graphics::PerspectiveCamera>();
    cam->transform.position += 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(1.0F, 0.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    const auto cam = manager.GetSingleton<graphics::PerspectiveCamera>();
    cam->transform.position -= 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(1.0F, 0.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    const auto cam = manager.GetSingleton<graphics::PerspectiveCamera>();
    cam->transform.position += 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    const auto cam = manager.GetSingleton<graphics::PerspectiveCamera>();
    cam->transform.position -= 10.0F * static_cast<float>(time::GetDeltaTime()) *
                               normalize(cam->transform.rotation * glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_E) == GLFW_PRESS) {
    manager.GetSingleton<graphics::PerspectiveCamera>()->transform.Rotate(
        100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
    manager.GetSingleton<graphics::PerspectiveCamera>()->transform.Rotate(
        -100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(0.0F, 1.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_F) == GLFW_PRESS) {
    manager.GetSingleton<graphics::PerspectiveCamera>()->transform.Rotate(
        100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(1.0F, 0.0F, 0.0F));
  }

  if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
    manager.GetSingleton<graphics::PerspectiveCamera>()->transform.Rotate(
        -100.0F * static_cast<float>(time::GetDeltaTime()), glm::vec3(1.0F, 0.0F, 0.0F));
  }
}

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
  glViewport(0, 0, width, height);
  if (manager.SingletonExists<graphics::OrthographicCamera>()) {
    manager.GetSingleton<graphics::OrthographicCamera>()->viewport_size = glm::vec2(width, height);
  } else if (manager.SingletonExists<graphics::PerspectiveCamera>()) {
    manager.GetSingleton<graphics::PerspectiveCamera>()->viewport_size = glm::vec2(width, height);
  }
}

void MouseCallback(GLFWwindow* window, const double x_pos, const double y_pos) {}
}  // namespace pixf
