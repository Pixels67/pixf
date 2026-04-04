#include "EventRegistry.hpp"

namespace Flock::Event {
    void EventRegistry::Add(const std::string &key, const Callback &callback) {
        m_Callbacks[key] = callback;
    }

    bool EventRegistry::Remove(const std::string &key) {
        if (m_Callbacks.contains(key)) {
            m_Callbacks.erase(key);
            return true;
        }

        return false;
    }

    bool EventRegistry::Invoke(const std::string &key) const {
        if (m_Callbacks.contains(key)) {
            m_Callbacks.at(key)();
            return true;
        }

        return false;
    }

    void EventRegistry::Clear() {
        m_Callbacks.clear();
    }
}
