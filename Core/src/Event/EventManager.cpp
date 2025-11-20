#include "EventManager.hpp"

namespace Pixf::Core::Event {
    void EventManager::ProcessEvents() {
        while (!GetInstance().m_EventQueue.empty()) {
            auto &[typeIndex, event] = GetInstance().m_EventQueue.front();

            if (auto it = GetInstance().m_Callbacks.find(typeIndex); it != GetInstance().m_Callbacks.end()) {
                for (auto &callback: it->second) {
                    callback(*event);
                }
            }

            for (auto &callback: GetInstance().m_GenericCallbacks) {
                callback(*event);
            }

            GetInstance().m_EventQueue.pop();
        }
    }

    void EventManager::Subscribe(const GenericCallback &callback) {
        GetInstance().m_GenericCallbacks.push_back(callback);
    }

    void EventManager::Clear() {
        while (!GetInstance().m_EventQueue.empty()) {
            GetInstance().m_EventQueue.pop();
        }
    }

    EventManager &EventManager::GetInstance() {
        static EventManager eventManager;
        return eventManager;
    }
} // namespace Pixf::Core::Event
