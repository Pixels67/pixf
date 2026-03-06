#include "EventHandler.hpp"

namespace Flock::Event {
    void EventQueue::QueueEvent(std::unique_ptr<Event> &&event) {
        m_Queue.push(std::move(event));
    }

    std::unique_ptr<Event> EventQueue::PopEvent() {
        std::unique_ptr<Event> ptr = std::move(m_Queue.front());
        m_Queue.pop();

        return ptr;
    }

    usize EventQueue::GetSize() const {
        return m_Queue.size();
    }

    bool EventQueue::IsEmpty() const {
        return m_Queue.empty();
    }

    void EventHandler::QueueEvent(std::unique_ptr<Event> &&event) {
        m_EventQueue.QueueEvent(std::move(event));
    }

    void EventHandler::Unsubscribe(const usize index) {
        m_FreeIndices.push_back(index);
    }

    void EventHandler::Update() {
        while (!m_EventQueue.IsEmpty()) {
            auto event = std::move(m_EventQueue.PopEvent());

            for (usize i = 0; i < m_Callbacks.size(); i++) {
                bool deleted = false;
                for (const auto fi : m_FreeIndices) {
                    if (i == fi) {
                        deleted = true;
                    }
                }

                if (deleted) {
                    continue;
                }

                m_Callbacks[i](*event);
                if (event->handled) {
                    break;
                }
            }
        }
    }
}
