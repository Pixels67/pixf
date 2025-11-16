#ifndef PIXF_EVENTMANAGER_HPP
#define PIXF_EVENTMANAGER_HPP

#include "Common.hpp"
#include "TypeId.hpp"

namespace Pixf::Core::Event {
    template<typename T>
    using EventCallback = std::function<void(T &)>;

    class PIXF_API EventManager {
    public:
        void ProcessEvents();
        void Clear();

        template<typename T>
        void Subscribe(EventCallback<T> callback) {
            const auto typeIndex = GetTypeId<T>();

            m_Callbacks[typeIndex].push_back([callback](std::any &e) { callback(std::any_cast<T &>(e)); });
        }

        template<typename T>
        void QueueEvent(T event = {}) {
            m_EventQueue.push({GetTypeId<T>(), std::make_any<T>(std::move(event))});
        }

        template<typename T>
        void DispatchEvent(T &event) {
            const auto typeIndex = GetTypeId<T>();

            if (const auto it = m_Callbacks.find(typeIndex); it != m_Callbacks.end()) {
                std::any eventAny = std::ref(event);
                for (auto &callback: it->second) {
                    callback(eventAny);
                }
            }
        }

        template<typename T>
        void DispatchEvent(T &&event) {
            T e = std::forward<T>(event);
            DispatchEvent(e);
        }

    private:
        using GenericCallback = std::function<void(std::any &)>;

        std::unordered_map<TypeId, std::vector<GenericCallback>> m_Callbacks;
        std::queue<std::pair<TypeId, std::any>> m_EventQueue;
    };
} // namespace Pixf::Core::Event

#endif // PIXF_EVENTMANAGER_HPP
