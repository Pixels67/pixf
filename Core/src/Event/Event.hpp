#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

namespace Pixf::Core::Event {
    class Event {
    public:
        virtual ~Event() = default;
        bool handled = false;
    };

    template<typename T>
    using EventCallback = std::function<void(T &)>;
    using GenericCallback = std::function<void(Event &)>;

    class EventManager {
    public:
        template<typename T>
        void Subscribe(EventCallback<T> callback) {
            const auto typeIndex = std::type_index(typeid(T));

            m_Callbacks[typeIndex].push_back([callback](Event &e) { callback(static_cast<T &>(e)); });
        }

        template<typename T>
        void QueueEvent(T event = {}) {
            m_EventQueue.push(std::make_unique<T>(std::move(event)));
        }

        template<typename T>
        void DispatchEvent(T &event) {
            const auto typeIndex = std::type_index(typeid(T));

            if (const auto it = m_Callbacks.find(typeIndex); it != m_Callbacks.end()) {
                for (auto &callback: it->second) {
                    callback(event);
                }
            }
        }

        template<typename T>
        void DispatchEvent(T &&event) {
            const auto typeIndex = std::type_index(typeid(T));
            T e = std::forward<T>(event);

            if (const auto it = m_Callbacks.find(typeIndex); it != m_Callbacks.end()) {
                for (auto &callback: it->second) {
                    callback(e);
                }
            }
        }

        void ProcessEvents() {
            while (!m_EventQueue.empty()) {
                auto &event = m_EventQueue.front();
                auto typeIndex = std::type_index(typeid(*event));

                if (auto it = m_Callbacks.find(typeIndex); it != m_Callbacks.end()) {
                    for (auto &callback: it->second) {
                        callback(*event);
                    }
                }

                m_EventQueue.pop();
            }
        }

        void Clear() { m_EventQueue = {}; }

    private:
        std::unordered_map<std::type_index, std::vector<GenericCallback>> m_Callbacks;
        std::queue<std::unique_ptr<Event>> m_EventQueue;
    };

} // namespace Pixf::Core::Event

#endif // EVENT_HPP
