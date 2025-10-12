#include "input_manager.h"

#include <GLFW/glfw3.h>

namespace pixf::input {
InputManager::InputManager(event::EventManager* event_manager, ui::RenderWindow* window)
    : event_manager_(event_manager), window_(window) {}

bool InputManager::IsKeyDown(const Key key) const {
  return glfwGetKey(window_->GetWindow(), static_cast<int>(key)) == GLFW_PRESS;
}

bool InputManager::IsKeyUp(const Key key) const { return !IsKeyDown(key); }

bool InputManager::IsMouseKeyDown(MouseKey key) const {
  return glfwGetMouseButton(window_->GetWindow(), static_cast<int>(key)) == GLFW_PRESS;
}

bool InputManager::IsMouseKeyUp(const MouseKey key) const { return !IsMouseKeyDown(key); }

glm::dvec2 InputManager::GetMousePosition() const { return mouse_pos_; }
glm::dvec2 InputManager::GetMouseDelta() const { return mouse_delta_; }

void InputManager::Update(ui::RenderWindow& window) {
  window_ = &window;
  glm::dvec2 old_mouse_pos = mouse_pos_;
  glfwGetCursorPos(window_->GetWindow(), &mouse_pos_.x, &mouse_pos_.y);
  mouse_delta_ = mouse_pos_ - old_mouse_pos;
}

}  // namespace pixf::input