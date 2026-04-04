#ifndef FLK_EVENTHANDLER_HPP
#define FLK_EVENTHANDLER_HPP

#include "Common.hpp"

namespace Flock::Event {
    class FLK_API Event {
    public:
        bool handled = false;

        virtual ~Event() = default;

        template<typename T>
        OptionalRef<T> Cast() {
            if (dynamic_cast<T *>(this)) {
                return dynamic_cast<T &>(*this);
            }

            return std::nullopt;
        }
    };

    using GenericCallback = std::function<void(Event &)>;

    class FLK_API EventQueue {
        std::queue<std::unique_ptr<Event> > m_Queue;

    public:
        EventQueue()  = default;
        ~EventQueue() = default;

        EventQueue(const EventQueue &other)     = delete;
        EventQueue(EventQueue &&other) noexcept = default;

        EventQueue &operator=(const EventQueue &other)     = delete;
        EventQueue &operator=(EventQueue &&other) noexcept = default;

        template<typename T>
        void QueueEvent(T &&event = {}) {
            auto ptr = std::make_unique<T>(std::forward<T>(event));
            m_Queue.push(std::move(ptr));
        }

        template<typename T>
        std::optional<T> PopEvent() {
            if (T event = m_Queue.back().get()->Cast<T>()) {
                m_Queue.pop();
                return event;
            }

            return std::nullopt;
        }

        void            QueueEvent(std::unique_ptr<Event> &&event = {});
        std::unique_ptr<Event> PopEvent();

        [[nodiscard]] usize GetSize() const;
        [[nodiscard]] bool  IsEmpty() const;
    };

    template<typename T>
    using EventCallback = std::function<void(T &)>;

    class FLK_API EventHandler {
        EventQueue                   m_EventQueue;
        std::vector<GenericCallback> m_Callbacks;
        std::vector<usize>           m_FreeIndices;

    public:
        template<typename T>
        void QueueEvent(T &&event = {}) {
            m_EventQueue.QueueEvent(std::forward<T>(event));
        }

        template<typename T>
        usize Subscribe(EventCallback<T> callback) {
            const GenericCallback func = [callback](Event &event) {
                if (event.Cast<T>()) {
                    callback(event.Cast<T>().value());
                }
            };

            if (!m_FreeIndices.empty()) {
                const usize idx  = m_FreeIndices.back();
                m_Callbacks[idx] = func;
                m_FreeIndices.pop_back();
                return idx;
            }

            m_Callbacks.push_back(func);
            return m_Callbacks.size() - 1;
        }

        void QueueEvent(std::unique_ptr<Event> &&event);

        void Unsubscribe(usize index);

        void Update();
    };
}

#endif //FLK_EVENTHANDLER_HPP
