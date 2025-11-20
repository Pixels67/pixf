#include "InputManager.hpp"

#include "Common.hpp"
#include "Graphics/Gl/Gl.hpp"
#include "Graphics/Gl/Window.hpp"

namespace Pixf::Core::Input {
    using namespace Graphics;

    void InputManager::Initialize() {
        Event::EventManager::Subscribe<Gl::KeyEvent>([](const Gl::KeyEvent &keyEvent) { HandleKeyEvent(keyEvent); });

        Event::EventManager::Subscribe<Gl::MouseKeyEvent>(
                [](const Gl::MouseKeyEvent &mouseKeyEvent) { HandleMouseKeyEvent(mouseKeyEvent); });

        Event::EventManager::Subscribe<Gl::MouseMovedEvent>(
                [](const Gl::MouseMovedEvent &mouseMovedEvent) { HandleMouseMovedEvent(mouseMovedEvent); });

        Event::EventManager::Subscribe<Gl::CharEvent>(
                [](const Gl::CharEvent &charEvent) { HandleCharEvent(charEvent); });
    }

    bool InputManager::IsKeyDown(Key key) {
        if (Gl::Window::GetCurrent()) {
            return Gl::Window::GetCurrent().value()->GetKey(static_cast<unsigned int>(key)) == GLFW_PRESS;
        }

        return false;
    }

    bool InputManager::IsMouseKeyDown(MouseKey mouseKey) {
        if (Gl::Window::GetCurrent()) {
            return Gl::Window::GetCurrent().value()->GetMouseKey(static_cast<unsigned int>(mouseKey)) == GLFW_PRESS;
        }

        return false;
    }

    Math::Vector2d InputManager::GetMousePosition() {
        if (Gl::Window::GetCurrent()) {
            const Math::Vector2d pos = Gl::Window::GetCurrent().value()->GetMousePosition();
            s_LastMousePosition = pos;
            return pos;
        }

        return Math::Vector2d::Zero();
    }

    Math::Vector2d InputManager::GetMouseDelta() {
        const Math::Vector2d lastPos = s_LastMousePosition;
        return GetMousePosition() - lastPos;
    }

    void InputManager::HandleKeyEvent(const Gl::KeyEvent &keyEvent) {
        const auto keyEnum = static_cast<Key>(keyEvent.keycode);
        const auto actionEnum = static_cast<KeyAction>(keyEvent.action);

        const auto event = KeyEvent(keyEnum, actionEnum);
        PIXF_CORE_LOG_TRACE("Key event: {}, {}", ToString(keyEnum), ToString(actionEnum));
        Event::EventManager::QueueEvent(event);
    }

    void InputManager::HandleMouseKeyEvent(const Gl::MouseKeyEvent &mouseKeyEvent) {
        const auto keyEnum = static_cast<MouseKey>(mouseKeyEvent.keycode);
        const auto actionEnum = static_cast<KeyAction>(mouseKeyEvent.action);

        const auto event = MouseKeyEvent(keyEnum, actionEnum);
        PIXF_CORE_LOG_TRACE("Mouse key event: {}, {}", ToString(keyEnum), ToString(actionEnum));
        Event::EventManager::QueueEvent(event);
    }

    void InputManager::HandleMouseMovedEvent(const Gl::MouseMovedEvent &mouseMovedEvent) {
        const auto pos = mouseMovedEvent.newPosition;
        const auto event = MouseMovedEvent(pos);
        PIXF_CORE_LOG_TRACE("Mouse position changed: {}, {}", pos.x, pos.y);
        Event::EventManager::QueueEvent(event);
    }

    void InputManager::HandleCharEvent(const Gl::CharEvent &charEvent) {
        const auto event = CharacterEvent(charEvent.codepoint);
        PIXF_CORE_LOG_TRACE("Character event: {}", static_cast<char>(charEvent.codepoint));
        Event::EventManager::QueueEvent(event);
    }

} // namespace Pixf::Core::Input
