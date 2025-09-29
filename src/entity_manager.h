#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace pixf {
struct Entity {
  unsigned int id = 0;
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

  template <typename T>
  void AddComponentToEntity(const Entity entity, T component) {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (entity.id >= components_[std::type_index(typeid(T))].size()) {
      components_[std::type_index(typeid(T))].resize(entity.id + 1);
    }

    components_[std::type_index(typeid(T))][entity.id] = std::make_shared<T>(component);
  }

  template <typename T>
  std::shared_ptr<T> GetEntityComponent(const Entity entity) {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    if (entity.id < components_[std::type_index(typeid(T))].size()) {
      return std::dynamic_pointer_cast<T>(components_[std::type_index(typeid(T))][entity.id]);
    }

    return nullptr;
  }

  template <typename T>
  std::vector<QueryElement<T>> Query() {
    static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
    std::vector<QueryElement<T>> query{};
    for (const auto& entity : entities_) {
      auto comp = GetEntityComponent<T>(entity);
      if (comp != nullptr) {
        query.push_back(QueryElement<T>{entity, comp});
      }
    }

    return query;
  }

 private:
  [[nodiscard]] unsigned int GenerateEntityId() const;

  std::vector<Entity> entities_;
  std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> components_;
};
}  // namespace pixf
