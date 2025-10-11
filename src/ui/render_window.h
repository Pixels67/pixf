#pragma once

#include <string>

struct GLFWwindow;

namespace pixf::ui {
struct WindowConfig {
  std::string title;
  unsigned int width;
  unsigned int height;
};

class RenderWindow {
 public:
  RenderWindow() = default;
  static RenderWindow CreateWindow(const std::string& title, unsigned int width,
                                   unsigned int height);

  [[nodiscard]] GLFWwindow* GetWindow() const;

  void Close() const;

  void SetTargetWindow() const;
  static void ClearRenderTarget();
  static void ResizeRenderTarget(unsigned int width, unsigned int height);

 private:
  std::string title_;
  GLFWwindow* window_ = nullptr;

  RenderWindow(const WindowConfig& config, GLFWwindow* window);
};
}  // namespace pixf::ui