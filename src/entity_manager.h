#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

namespace Engine {
    struct Entity {
        unsigned int id = 0;
    };

    struct Component {
        virtual ~Component() = default;
    };

    template<typename T>
    struct Query {
        std::vector<std::shared_ptr<T> > components;
    };

    class EntityManager {
    public:
         unsigned int CreateEntity();

        template<typename T>
        void AddComponentToEntity(unsigned int entityId, T component) {
            static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
            if (entityId >= m_Components[std::type_index(typeid(T))].size()) {
                m_Components[std::type_index(typeid(T))].resize(entityId + 1);
            }
            
            m_Components[std::type_index(typeid(T))][entityId] = std::make_shared<T>(component);
        }

        template<typename T>
        T &GetEntityComponent(unsigned int entityId) {
            static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
            return *m_Components[std::type_index(typeid(T))][entityId].get();
        }

        template<typename T>
        Query<T> GetQuery() {
            static_assert(std::is_base_of<Component, T>(), "T must derive from Component");
            Query<T> query;
            for (auto comp : m_Components[std::type_index(typeid(T))]) {
                query.components.push_back(std::dynamic_pointer_cast<T>(comp));
            }

            return query;
        }

    private:
        unsigned int GenerateEntityId();
        
        std::vector<Entity> m_Entities;
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component> > > m_Components;
    };
}
