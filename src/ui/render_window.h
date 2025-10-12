#pragma once

#include <string>

struct GLFWwindow;

namespace pixf::ui {

#define GLFW_CURSOR_NORMAL 0x00034001
#define GLFW_CURSOR_HIDDEN 0x00034002
#define GLFW_CURSOR_DISABLED 0x00034003
#define GLFW_CURSOR_CAPTURED 0x00034004

enum class CursorMode : uint32_t {
  NORMAL = GLFW_CURSOR_NORMAL,
  HIDDEN = GLFW_CURSOR_HIDDEN,
  DISABLED = GLFW_CURSOR_DISABLED,
  CAPTURED = GLFW_CURSOR_CAPTURED,
};

struct WindowConfig {
  std::string title;
  unsigned int width = 600;
  unsigned int height = 800;
  unsigned int sample_count = 4;
  CursorMode cursor_mode = CursorMode::NORMAL;
  bool v_sync = true;
};

class RenderWindow {
 public:
  RenderWindow() = default;
  explicit RenderWindow(const WindowConfig& config);

  [[nodiscard]] GLFWwindow* GetWindow() const;

  void Close();
  void SetTargetWindow() const;
  void SwapBuffers() const;

  bool IsRunning() const;

  static void ClearRenderTarget();
  static void ResizeRenderTarget(unsigned int width, unsigned int height);

 private:
  std::string title_;
  GLFWwindow* window_ = nullptr;
  bool is_running_ = false;
};
}  // namespace pixf::ui