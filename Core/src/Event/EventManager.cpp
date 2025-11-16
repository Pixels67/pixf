#include "EventManager.hpp"

namespace Pixf::Core::Event {
    void EventManager::ProcessEvents() {
        while (!m_EventQueue.empty()) {
            auto &[typeIndex, event] = m_EventQueue.front();

            if (auto it = m_Callbacks.find(typeIndex); it != m_Callbacks.end()) {
                for (auto &callback: it->second) {
                    callback(event);
                }
            }

            m_EventQueue.pop();
        }
    }

    void EventManager::Clear() {
        while (!m_EventQueue.empty()) {
            m_EventQueue.pop();
        }
    }
} // namespace Pixf::Core::Event
