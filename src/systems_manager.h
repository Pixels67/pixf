#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "entity_manager.h"

namespace engine {
struct System {
  virtual ~System() = default;

  virtual void OnInit(EntityManager& entity_manager) = 0;
  virtual void OnUpdate(EntityManager& entity_manager, double delta_time) = 0;
};

class SystemsManager {
 public:
  template <typename T>
  void AddSystem() {
    static_assert(std::is_base_of<System, T>(), "T must derive from System");
    systems_.push_back(std::make_unique<T>());
  }

  void InitSystems(EntityManager& entity_manager) const;
  void UpdateSystems(EntityManager& entity_manager, double delta_time) const;

 private:
  std::vector<std::unique_ptr<System> > systems_;
};
}  // namespace engine
