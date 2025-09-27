#pragma once

#include <string>

struct GLFWwindow;

namespace engine::ui {
struct WindowConfig {
  std::string title;
  unsigned int width;
  unsigned int height;
};

class RenderWindow {
 public:
  static RenderWindow CreateWindow(const std::string& title, unsigned int width,
                                   unsigned int height);

  RenderWindow() = default;

  [[nodiscard]] GLFWwindow* GetWindow() const;

  void Close() const;

 private:
  std::string title_;
  unsigned int width_ = 0;
  unsigned int height_ = 0;
  GLFWwindow* window_ = nullptr;

  RenderWindow(const WindowConfig& config, GLFWwindow* window);
};
}  // namespace engine::ui