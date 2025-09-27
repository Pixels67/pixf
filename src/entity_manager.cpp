#include "entity_manager.h"

namespace engine {
Entity EntityManager::CreateEntity() {
  const unsigned int id = GenerateEntityId();
  Entity entity;
  entity.id = id;
  entities_.push_back(entity);
  return entity;
}

unsigned int EntityManager::GenerateEntityId() const { return entities_.size(); }
}  // namespace engine
