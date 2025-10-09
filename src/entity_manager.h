#pragma once

#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace pixf {
struct Entity {
  bool operator==(const Entity &entity) const;
  bool operator!=(const Entity &entity) const;

 private:
  friend class EntityManager;

  unsigned int id = 0;
  unsigned int version = 0;
  bool active = true;
};

struct Component {
  virtual ~Component() = default;
};

template <typename T>
struct QueryElement {
  Entity entity;
  std::shared_ptr<T> component;
};

class EntityManager {
 public:
  Entity CreateEntity();
  void DestroyEntity(Entity entity);

  template <typename T>
  void AddComponentToEntity(const Entity entity, T component) {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (!IsEntityValid(entity) || !entity.active) {
      std::cerr << "Entity is not valid!\n";
      return;
    }

    if (entity.id >= components_[std::type_index(typeid(T))].size()) {
      components_[std::type_index(typeid(T))].resize(entity.id + 1);
    }

    components_[std::type_index(typeid(T))][entity.id] = std::make_shared<T>(component);
  }

  template <typename T>
  std::shared_ptr<T> GetEntityComponent(const Entity entity) {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (!IsEntityValid(entity) || !entity.active) {
      std::cerr << "Entity is not valid!\n";
      return nullptr;
    }

    if (entity.id >= components_[std::type_index(typeid(T))].size()) {
      std::cerr << "Component not found!\n";
      return nullptr;
    }

    return std::dynamic_pointer_cast<T>(components_[std::type_index(typeid(T))][entity.id]);
  }

  template <typename T>
  void RemoveEntityComponent(const Entity entity) {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");

    if (!IsEntityValid(entity) || !entity.active) {
      std::cerr << "Entity is not valid!\n";
      return;
    }

    if (entity.id >= components_[std::type_index(typeid(T))].size()) {
      std::cerr << "Component not found!\n";
      return;
    }

    components_[std::type_index(typeid(T))][entity.id] = nullptr;
  }

  template <typename T>
  bool IsComponentRegistered() {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");

    return components_.find(std::type_index(typeid(T))) != components_.end();
  }

  template <typename T>
  std::vector<QueryElement<T>> Query() {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");

    std::vector<QueryElement<T>> query{};
    const auto type = std::type_index(typeid(T));

    if (components_.find(type) == components_.end()) {
      std::cerr << "Component is not registered!\n";
      return query;
    }

    const std::vector comps = components_[type];

    for (const auto &entity : entities_) {
      if (!entity.active) {
        continue;
      }

      if (entity.id >= comps.size()) continue;
      if (auto comp = comps[entity.id]; comp != nullptr) {
        query.push_back(QueryElement<T>{entity, std::dynamic_pointer_cast<T>(comp)});
      }
    }

    return query;
  }

  template <typename T>
  void CreateSingleton(T component) {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (singletons_.find(std::type_index(typeid(T))) != singletons_.end()) {
      std::cerr << "Singleton already exists!\n";
      return;
    }

    singletons_[std::type_index(typeid(T))] = std::make_shared<T>(component);
  }

  template <typename T>
  std::shared_ptr<T> GetSingleton() {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (singletons_.find(std::type_index(typeid(T))) == singletons_.end()) {
      std::cerr << "Singleton does not exist!\n";
      return nullptr;
    }

    return std::dynamic_pointer_cast<T>(singletons_[std::type_index(typeid(T))]);
  }

  template <typename T>
  void DestroySingleton() {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (singletons_.find(std::type_index(typeid(T))) == singletons_.end()) {
      std::cerr << "Singleton does not exist!\n";
      return;
    }

    singletons_.erase(std::type_index(typeid(T)));
  }

  template <typename T>
  bool IsSingletonRegistered() {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (singletons_.find(std::type_index(typeid(T))) == singletons_.end()) {
      return false;
    }

    return singletons_[std::type_index(typeid(T))] != nullptr;
  }

 private:
  [[nodiscard]] unsigned int GenerateEntityId() const;
  [[nodiscard]] bool IsEntityValid(Entity entity) const;

  std::vector<Entity> entities_;
  std::unordered_map<std::type_index, std::shared_ptr<Component>> singletons_;
  std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> components_;
};
}  // namespace pixf
