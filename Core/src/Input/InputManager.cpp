#include "InputManager.hpp"

#include "../Common.hpp"
#include "Graphics/Gl/Gl.hpp"
#include "Graphics/Gl/Window.hpp"

namespace Pixf::Core::Input {
    InputManager::InputManager(Event::EventManager &eventManager, Graphics::Gl::Window &window) :
        m_EventManager(&eventManager), m_Window(&window) {}

    bool InputManager::IsKeyDown(const Key key) const {
        return glfwGetKey(m_Window->GetGlfwWindowPtr(), static_cast<int>(key)) == GLFW_PRESS;
    }

    bool InputManager::IsKeyUp(const Key key) const { return !IsKeyDown(key); }

    bool InputManager::IsMouseKeyDown(MouseKey key) const {
        return glfwGetMouseButton(m_Window->GetGlfwWindowPtr(), static_cast<int>(key)) == GLFW_PRESS;
    }

    bool InputManager::IsMouseKeyUp(const MouseKey key) const { return !IsMouseKeyDown(key); }

    dvec2 InputManager::GetMousePosition() const { return m_MousePos; }

    dvec2 InputManager::GetMouseDelta() const { return m_MouseDelta; }

    void InputManager::Update() {
        const dvec2 oldMousePos = m_MousePos;
        glfwGetCursorPos(m_Window->GetGlfwWindowPtr(), &m_MousePos.x, &m_MousePos.y);
        m_MouseDelta = m_MousePos - oldMousePos;
    }

} // namespace Pixf::Core::Input
