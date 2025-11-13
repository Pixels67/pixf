#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "ComponentManager.hpp"
#include "Json/Json.hpp"
#include "Serialization/Serializable.hpp"
#include "Common.hpp"

namespace Pixf::Core::Entities {
    struct PIXF_API Entity final : Serialization::Serializable {
        friend class EntityManager;
        explicit Entity() = default;
        explicit Entity(const unsigned int id, const std::string &name) : name(name), id(id), active(true) {}

        std::string GetName() const { return name; }
        unsigned int GetId() const { return id; }
        uint8_t GetVersion() const { return version; }
        bool IsActive() const { return active; }

        Json::object Serialize() override {
            Json::object json;

            json["name"] = name;
            json["id"] = id;
            json["version"] = version;
            json["active"] = active;

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            name = json.at("name").as_string().c_str();
            id = json.at("id").to_number<unsigned int>();
            version = json.at("version").to_number<uint8_t>();
            active = json.at("active").as_bool();
        }

        bool operator==(const Entity &other) const { return id == other.id && version == other.version; }
        bool operator!=(const Entity &other) const { return !(*this == other); }

    private:
        std::string name = "";
        unsigned int id = 0;
        uint8_t version = 0;
        bool active = false;
    };

    enum class SingletonError : uint8_t {
        None = 0,
        AlreadyExists,
        NotFound,
    };

    class PIXF_API EntityManager final : Serialization::Serializable {
    public:
        EntityManager() = default;

        EntityManager(const EntityManager &) = default;
        EntityManager(EntityManager &&) = delete;
        EntityManager &operator=(const EntityManager &) = default;
        EntityManager &operator=(EntityManager &&) = delete;

        ~EntityManager() override = default;

        Entity CreateEntity(const std::string &name = "Unnamed");

        std::optional<Entity> GetEntity(unsigned int id) const;
        std::vector<Entity> GetAllEntities() const;

        void DestroyEntity(Entity entity);
        void Clear();

        template<TypeInformed T>
        void RegisterComponent() {
            m_ComponentManager.RegisterComponent<T>();
        }

        template<TypeInformed T>
        void AddComponent(const Entity entity, const T component = T()) {
            m_ComponentManager.AddComponent<T>(entity.id, component);
        }

        template<TypeInformed T>
        Entity CreateEntityWithComponent(const T component = T()) {
            const Entity entity = CreateEntity();
            AddComponent<T>(entity, component);
            return entity;
        }

        template<TypeInformed T>
        bool HasComponent(const Entity entity) {
            return m_ComponentManager.HasComponent<T>(entity.id);
        }

        template<TypeInformed T>
        Error::Result<std::shared_ptr<T>, ComponentError> GetComponent(const Entity entity) {
            return m_ComponentManager.GetComponent<T>(entity.id);
        }

        template<TypeInformed T>
        void RemoveComponent(const Entity entity) {
            m_ComponentManager.RemoveComponent<T>(entity.id);
        }

        void ClearComponents(const Entity &entity) const { m_ComponentManager.ClearComponents(entity.id); }

        template<TypeInformed... T>
        std::vector<Entity> GetEntitiesWith() {
            std::vector<Entity> result;

            for (auto entity: m_Entities) {
                if ((HasComponent<T>(entity) && ...)) {
                    result.push_back(entity);
                }
            }

            return result;
        }

        template<TypeInformed... T, typename Func>
        void ForEach(Func &&func) {
            for (auto entity: m_Entities) {
                if ((HasComponent<T>(entity) && ...)) {
                    func(*GetComponent<T>(entity).Unwrap()...);
                }
            }
        }

        template<typename Func>
        void ForEachEntity(Func &&func) {
            for (auto entity: m_Entities) {
                func(entity);
            }
        }

        template<TypeInformed... T, typename Func>
        void ForEachEntity(Func &&func) {
            for (auto entity: m_Entities) {
                if ((HasComponent<T>(entity) && ...)) {
                    func(entity, *GetComponent<T>(entity).Unwrap()...);
                }
            }
        }

        template<TypeInformed T>
        Error::Result<std::shared_ptr<ComponentRegistry<T>>, ComponentError> GetRegistry() {
            return m_ComponentManager.QueryComponents<T>();
        }

        template<TypeInformed T>
        SingletonError CreateSingleton(T singleton = T()) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (m_Singletons.contains(T::GetTypeId())) {
                return SingletonError::AlreadyExists;
            }

            auto ptr = std::make_shared<T>(singleton);
            m_Singletons[T::GetTypeId()] = ptr;

            return SingletonError::None;
        }

        template<TypeInformed T>
        bool HasSingleton() const {
            return m_Singletons.contains(T::GetTypeId());
        }

        template<TypeInformed T>
        Error::Result<std::shared_ptr<T>, SingletonError> GetSingleton() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (!HasSingleton<T>()) {
                return SingletonError::NotFound;
            }

            return std::static_pointer_cast<T>(m_Singletons[T::GetTypeId()]);
        }

        template<TypeInformed T>
        void DestroySingleton() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            if (!HasSingleton<T>()) {
                return;
            }

            m_Singletons.erase(T::GetTypeId());
        }

        ComponentManager &GetComponentManager() { return m_ComponentManager; }

        Json::object Serialize() override {
            Json::object json;

            json["entities"] = Json::array();

            for (auto &entity: m_Entities) {
                json["entities"].as_array().push_back(entity.Serialize());
            }

            json["components"] = m_ComponentManager.Serialize();

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            Clear();

            for (Json::array entities = json.at("entities").as_array(); auto &entity: entities) {
                Entity e{};
                e.Deserialize(entity.as_object(), assetManager);
                m_Entities.emplace_back(std::move(e));
            }

            m_ComponentManager.Deserialize(json.at("components").as_object(), assetManager);
        }

    private:
        std::vector<Entity> m_Entities;
        std::unordered_map<uint64_t, std::shared_ptr<Component>> m_Singletons;
        ComponentManager m_ComponentManager;
        unsigned int m_EntityIdCounter = 0;

        size_t GenEntityId();
        std::optional<Entity *> GetFirstInactiveEntity();
    };

} // namespace Pixf::Core::Entities

#endif // ENTITYMANAGER_HPP
