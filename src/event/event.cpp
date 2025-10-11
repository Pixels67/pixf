#include "event.h"

namespace pixf::event {

void EventQueue::Enqueue(const std::shared_ptr<Event>& event) { events.push(event); }

std::shared_ptr<Event> EventQueue::Pop() {
  if (events.empty()) {
    return {};
  }

  std::shared_ptr<Event> event = events.front();
  events.pop();
  return event;
}

void EventQueue::Clear() {
  const size_t size = events.size();
  for (size_t i = 0; i < size; i++) {
    events.pop();
  }
}
void EventManager::Subscribe(EventListener* listener) { listeners.push_back(listener); }

void EventManager::Unsubscribe(EventListener* listener) {
  listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void EventManager::DispatchEvents() {
  while (!events.events.empty()) {
    DispatchEvent(events.Pop());
  }
}

void EventManager::DispatchEvent(const std::shared_ptr<Event>& event) const {
  for (const auto& listener : listeners) {
    listener->OnEvent(event);
  }
}

void EventManager::QueueEvent(const std::shared_ptr<Event>& event) { events.Enqueue(event); }
}  // namespace pixf::event