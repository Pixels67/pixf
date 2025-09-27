#include "systems_manager.h"

#include <vector>

#include "entity_manager.h"

namespace engine {
void SystemsManager::InitSystems(EntityManager& entity_manager) const {
  for (const auto& system : systems_) {
    system->OnInit(entity_manager);
  }
}

void SystemsManager::UpdateSystems(EntityManager& entity_manager,
                                   double delta_time) const {
  for (const auto& system : systems_) {
    system->OnUpdate(entity_manager, delta_time);
  }
}
}  // namespace engine
