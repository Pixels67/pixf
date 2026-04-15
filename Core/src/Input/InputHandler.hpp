#ifndef FLK_INPUTHANDLER_HPP
#define FLK_INPUTHANDLER_HPP

#include "Input.hpp"
#include "Event/EventHandler.hpp"

namespace Flock {
namespace Event {
class EventHandler;
}  // namespace Event
}  // namespace Flock

namespace Flock::Input {
    class InputHandler {
        InputState m_InputState;

    public:
        void HookEvents(Event::EventHandler &eventHandler);
        void ResetState();

        [[nodiscard]] InputState State() const;
    };
}

#endif //FLK_INPUTHANDLER_HPP
