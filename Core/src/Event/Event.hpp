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
    };

    template<typename T>
    using EventCallback = std::function<void(const T &)>;

    using GenericCallback = std::function<void(const Event &)>;

    class EventManager {
    public:
        template<typename T>
        void Subscribe(EventCallback<T> callback) {
            const auto typeIndex = std::type_index(typeid(T));

            m_Listeners[typeIndex].push_back([callback](const Event &e) { callback(static_cast<const T &>(e)); });
        }

        template<typename T>
        void QueueEvent(T event) {
            m_EventQueue.push(std::make_unique<T>(std::move(event)));
        }

        template<typename T>
        void DispatchEvent(const T &event) {
            const auto typeIndex = std::type_index(typeid(T));

            if (const auto it = m_Listeners.find(typeIndex); it != m_Listeners.end()) {
                for (auto &callback: it->second) {
                    callback(event);
                }
            }
        }

        void ProcessEvents() {
            while (!m_EventQueue.empty()) {
                auto &event = m_EventQueue.front();
                auto typeIndex = std::type_index(typeid(*event));

                if (auto it = m_Listeners.find(typeIndex); it != m_Listeners.end()) {
                    for (auto &callback: it->second) {
                        callback(*event);
                    }
                }

                m_EventQueue.pop();
            }
        }

        void Clear() { m_EventQueue = {}; }

    private:
        std::unordered_map<std::type_index, std::vector<GenericCallback>> m_Listeners;
        std::queue<std::unique_ptr<Event>> m_EventQueue;
    };

} // namespace Pixf::Core::Event

#endif // EVENT_HPP
