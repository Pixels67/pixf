#include "render_window.h"

#include <glad/glad.h>
// ReSharper disable once CppWrongIncludesOrder
#include <GLFW/glfw3.h>

#include <iostream>

constexpr unsigned int GL_VERSION_MAJOR = 3;
constexpr unsigned int GL_VERSION_MINOR = 3;
constexpr unsigned int SAMPLE_COUNT = 4;

namespace pixf::ui {
RenderWindow RenderWindow::CreateWindow(const std::string& title, const unsigned int width,
                                        const unsigned int height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, SAMPLE_COUNT);

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
                                        title.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window!\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
    std::cerr << "Failed to initialize GLAD!\n";
    std::exit(EXIT_FAILURE);
  }

  const WindowConfig config{title, width, height};

  glfwMakeContextCurrent(nullptr);

  return {config, window};
}

GLFWwindow* RenderWindow::GetWindow() const { return window_; }

void RenderWindow::Close() const { glfwSetWindowShouldClose(window_, 1); }

void RenderWindow::SetTargetWindow() const { glfwMakeContextCurrent(window_); }
void RenderWindow::ClearRenderTarget() { glfwMakeContextCurrent(nullptr); }
void RenderWindow::ResizeRenderTarget(const unsigned int width, const unsigned int height) {
  glViewport(0, 0, width, height);
}

RenderWindow::RenderWindow(const WindowConfig& config, GLFWwindow* window)
    : title_(config.title), window_(window) {}
}  // namespace pixf::ui
