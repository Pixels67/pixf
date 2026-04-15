#include "InputHandler.hpp"

#include <set>

#include "Glfw/Window.hpp"
#include "Event/EventHandler.hpp"
#include "Input/Input.hpp"
#include "Math/Vector.hpp"

namespace Flock::Input {
    void InputHandler::HookEvents(Event::EventHandler &eventHandler) {
        eventHandler.Subscribe<Glfw::KeyEvent>([&](auto &event) {
            const Key    key    = KeyFromGlfwType(event.key);
            const Action action = ActionFromGlfwType(event.action);

            switch (action) {
                case Action::Press:
                    m_InputState.heldKeys.emplace(key);
                    m_InputState.pressedKeys.emplace(key);
                    break;
                case Action::Release:
                    m_InputState.heldKeys.erase(key);
                    m_InputState.releasedKeys.emplace(key);
                    break;
                default:
                    break;
            }

            eventHandler.QueueEvent(KeyEvent(key, action));
            event.handled = true;
        });

        eventHandler.Subscribe<Glfw::MouseButtonEvent>([&](auto &event) {
            const MouseButton button = MouseButtonFromGlfwType(event.button);
            const Action      action = ActionFromGlfwType(event.action);

            switch (action) {
                case Action::Press:
                    m_InputState.heldMouseButtons.emplace(button);
                    m_InputState.pressedMouseButtons.emplace(button);
                    break;
                case Action::Release:
                    m_InputState.heldMouseButtons.erase(button);
                    m_InputState.releasedMouseButtons.emplace(button);
                    break;
                default:
                    break;
            }

            eventHandler.QueueEvent(MouseButtonEvent(button, action));
            event.handled = true;
        });

        eventHandler.Subscribe<Glfw::CursorPositionEvent>([&](auto &event) {
            if (m_InputState.cursorPosition != Vector2f{}) {
                m_InputState.cursorDelta = Vector2d{event.xPos, event.yPos} - m_InputState.cursorPosition;
            }

            m_InputState.cursorPosition.x = event.xPos;
            m_InputState.cursorPosition.y = event.yPos;

            eventHandler.QueueEvent(CursorPositionEvent(m_InputState.cursorPosition));
            event.handled = true;
        });

        eventHandler.Subscribe<Glfw::MouseScrollEvent>([&](auto &event) {
            m_InputState.scrollOffset.x = event.xOffset;
            m_InputState.scrollOffset.y = event.yOffset;

            eventHandler.QueueEvent(MouseScrollEvent(m_InputState.scrollOffset));
            event.handled = true;
        });
    }

    void InputHandler::ResetState() {
        m_InputState.pressedKeys.clear();
        m_InputState.releasedKeys.clear();
        m_InputState.pressedMouseButtons.clear();
        m_InputState.releasedMouseButtons.clear();
        m_InputState.cursorDelta = {};
    }

    InputState InputHandler::State() const {
        return m_InputState;
    }
}
