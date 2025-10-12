#include "render_window.h"

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>

#include <iostream>

constexpr unsigned int GL_VERSION_MAJOR = 3;
constexpr unsigned int GL_VERSION_MINOR = 3;

namespace pixf::ui {
RenderWindow::RenderWindow(const WindowConfig& config) : title_(config.title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, static_cast<int>(config.sample_count));

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window =
      glfwCreateWindow(static_cast<int>(config.width), static_cast<int>(config.height),
                       config.title.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window!\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(config.v_sync ? 1 : 0);
  glfwSetInputMode(window, GLFW_CURSOR, static_cast<int>(config.cursor_mode));

  if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
    std::cerr << "Failed to initialize GLAD!\n";
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(nullptr);
  window_ = window;
  is_running_ = true;
}

GLFWwindow* RenderWindow::GetWindow() const { return window_; }

void RenderWindow::Close() {
  glfwSetWindowShouldClose(window_, 1);
  is_running_ = false;
}

void RenderWindow::SetTargetWindow() const { glfwMakeContextCurrent(window_); }
void RenderWindow::SwapBuffers() const { glfwSwapBuffers(window_); }
bool RenderWindow::IsRunning() const { return glfwWindowShouldClose(window_) == 0 && is_running_; }
void RenderWindow::ClearRenderTarget() { glfwMakeContextCurrent(nullptr); }
void RenderWindow::ResizeRenderTarget(const unsigned int width, const unsigned int height) {
  glViewport(0, 0, width, height);
}
}  // namespace pixf::ui
