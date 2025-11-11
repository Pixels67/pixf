#include "InputManager.hpp"

#include "../Common.hpp"
#include "Window.hpp"
#include "Graphics/Gl/Gl.hpp"

namespace Pixf::Core::Input {
    InputManager::InputManager(Event::EventManager &eventManager, Window &window) :
        m_EventManager(&eventManager), m_Window(&window) {
        SetupCallbacks(window);
    }

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

    void InputManager::SetupCallbacks(const Window &window) const {
        glfwSetWindowUserPointer(window.GetGlfwWindowPtr(), m_EventManager);

        // Key input
        glfwSetKeyCallback(window.GetGlfwWindowPtr(), [](GLFWwindow *window, const int key, const int scancode, const int action, const int mods) {
            const auto eventManager = static_cast<Event::EventManager *>(glfwGetWindowUserPointer(window));
            const auto event = KeyEvent(static_cast<Key>(key), static_cast<KeyAction>(action));
            PIXF_LOG_TRACE("Key event: ", static_cast<Key>(key), ", ", static_cast<KeyAction>(action));
            eventManager->QueueEvent(event);
        });

        // Mouse moved
        glfwSetCursorPosCallback(window.GetGlfwWindowPtr(), [](GLFWwindow *window, const double xPos, const double yPos) {
            const auto eventManager = static_cast<Event::EventManager *>(glfwGetWindowUserPointer(window));
            const auto event = MouseMovedEvent(vec2(xPos, yPos));
            PIXF_LOG_TRACE("Mouse position changed: ", xPos, ", ", yPos);
            eventManager->QueueEvent(event);
        });

        // Mouse click
        glfwSetMouseButtonCallback(window.GetGlfwWindowPtr(), [](GLFWwindow *window, int button, int action, int mods) {
            const auto eventManager = static_cast<Event::EventManager *>(glfwGetWindowUserPointer(window));
            const auto event = MouseKeyEvent(static_cast<MouseKey>(button), static_cast<KeyAction>(action));
            PIXF_LOG_TRACE("Mouse key event: ", static_cast<MouseKey>(button), ", ", static_cast<KeyAction>(action));
            eventManager->QueueEvent(event);
        });

        // Character input
        glfwSetCharCallback(window.GetGlfwWindowPtr(), [](GLFWwindow *window, const unsigned int codepoint) {
            const auto eventManager = static_cast<Event::EventManager *>(glfwGetWindowUserPointer(window));
            const auto event = CharacterEvent(codepoint);
            PIXF_LOG_TRACE("Character event: ", static_cast<char>(codepoint));
            eventManager->QueueEvent(event);
        });
    }

} // namespace Pixf::Core::Input
