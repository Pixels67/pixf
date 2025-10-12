#pragma once

#include <memory>
#include <queue>

#include "entity_manager.h"

namespace pixf::event {

class Event {
 public:
  virtual ~Event() = default;
};

class EventListener {
 public:
  virtual ~EventListener() = default;
  virtual void OnEvent(std::shared_ptr<Event> event) = 0;
};

class EventQueue {
 public:
  std::queue<std::shared_ptr<Event>> events;

  void Enqueue(const std::shared_ptr<Event>& event);
  std::shared_ptr<Event> Pop();

  void Clear();
};

struct EventManager final : Component {
  void Subscribe(EventListener* listener);
  void Unsubscribe(EventListener* listener);

  void DispatchEvents();
  void DispatchEvent(const std::shared_ptr<Event>& event) const;
  void QueueEvent(const std::shared_ptr<Event>& event);

 private:
  EventQueue events = {};
  std::vector<EventListener*> listeners;
};

}  // namespace pixf::event