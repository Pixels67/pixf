#ifndef PIXF_EVENTMANAGER_HPP
#define PIXF_EVENTMANAGER_HPP

#include "Common.hpp"
#include "Event.hpp"
#include "TypeId.hpp"

namespace Pixf::Core::Event {
    template<typename T>
    using EventCallback = std::function<void(T &)>;
    using GenericCallback = std::function<void(Event &)>;

    class PIXF_API EventManager {
    public:
        EventManager() = default;

        EventManager(const EventManager &) = delete;
        EventManager(EventManager &&) noexcept = default;
        EventManager &operator=(const EventManager &) = delete;
        EventManager &operator=(EventManager &&) noexcept = default;

        ~EventManager() = default;

        void ProcessEvents();
        void Subscribe(const GenericCallback& callback);

        void Clear();

        template<typename T>
        void Subscribe(EventCallback<T> callback) {
            static_assert(std::is_base_of<Event, T>());

            const auto typeIndex = GetTypeId<T>();

            m_Callbacks[typeIndex].push_back([callback](Event &e) { callback(static_cast<T &>(e)); });
        }

        template<typename T>
        void QueueEvent(T event = {}) {
            static_assert(std::is_base_of<Event, T>());

            auto ptr = std::make_unique<T>(event);
            QueueElement element = std::make_pair(GetTypeId<T>(), std::move(ptr));
            m_EventQueue.push(std::move(element));
        }

        template<typename T>
        void DispatchEvent(T event = {}) {
            static_assert(std::is_base_of<Event, T>());

            const auto typeIndex = GetTypeId<T>();

            if (const auto it = m_Callbacks.find(typeIndex); it != m_Callbacks.end()) {
                for (auto &callback: it->second) {
                    callback(event);
                }
            }
        }

    private:
        using QueueElement = std::pair<TypeId, std::unique_ptr<Event>>;

        std::vector<GenericCallback> m_GenericCallbacks;
        std::unordered_map<TypeId, std::vector<GenericCallback>> m_Callbacks;
        std::queue<QueueElement> m_EventQueue;
    };
} // namespace Pixf::Core::Event

#endif // PIXF_EVENTMANAGER_HPP
