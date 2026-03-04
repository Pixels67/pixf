#ifndef FLK_INPUTHANDLER_HPP
#define FLK_INPUTHANDLER_HPP

#include "Input.hpp"
#include "Event/EventHandler.hpp"

namespace Flock::Input {
    class InputHandler {
        InputState m_InputState;

    public:
        void HookEvents(Event::EventHandler &eventHandler);
        void ResetState();

        [[nodiscard]] InputState GetState() const;
    };
}

#endif //FLK_INPUTHANDLER_HPP
