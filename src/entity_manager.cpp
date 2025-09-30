#include "entity_manager.h"

namespace pixf {
bool Entity::operator==(const Entity &entity) const {
  return entity.id == id && entity.version == version && entity.active == active;
}

bool Entity::operator!=(const Entity &entity) const { return !(entity == *this); }

Entity EntityManager::CreateEntity() {
  for (unsigned int i = 0; i < entities_.size(); i++) {
    if (!entities_[i].active) {
      entities_[i].active = true;
      entities_[i].version++;
      return entities_[i];
    }
  }

  const unsigned int id = GenerateEntityId();
  Entity entity;
  entity.id = id;
  entity.version = 0;
  entity.active = true;
  entities_.push_back(entity);
  return entity;
}

void EntityManager::DestroyEntity(const Entity entity) {
  for (unsigned int i = 0; i < entities_.size(); i++) {
    if (entities_[i] == entity) {
      entities_[i].active = false;
      for (auto &[_, vec] : components_) {
        vec[entities_[i].id] = nullptr;
      }

      return;
    }
  }
}

unsigned int EntityManager::GenerateEntityId() const { return entities_.size(); }

bool EntityManager::IsEntityValid(const Entity entity) const {
  if (!entity.active) return false;

  for (unsigned int i = 0; i < entities_.size(); i++) {
    if (entity == entities_[i] && entities_[i].active) {
      return true;
    }
  }

  return false;
}
}  // namespace pixf
