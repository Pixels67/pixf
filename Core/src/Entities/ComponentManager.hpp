#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#include <typeindex>
#include <unordered_map>

#include "ComponentRegistry.hpp"
#include "Error/Result.hpp"

namespace Pixf::Core::Entities {
    enum class ComponentError : uint8_t {
        None = 0,
        NotFound,
        NotRegistered,
    };

    class ComponentManager {
    public:
        ComponentManager() = default;

        ComponentManager(const ComponentManager &) = default;
        ComponentManager(ComponentManager &&) = delete;
        ComponentManager &operator=(const ComponentManager &) = default;
        ComponentManager &operator=(ComponentManager &&) = delete;

        ~ComponentManager() = default;

        template<typename T>
        void RegisterComponent() {
            m_Registries[GetTypeIndex<T>()] =
                    std::dynamic_pointer_cast<IComponentRegistry>(std::make_shared<ComponentRegistry<T>>());
        }

        template<typename T>
        void AddComponent(size_t index) {
            if (m_Registries.find(GetTypeIndex<T>()) == m_Registries.end()) {
                RegisterComponent<T>();
            }

            m_Registries[GetTypeIndex<T>()]->Add(index);
        }

        template<typename T>
        void AddComponent(size_t index, T component) {
            if (m_Registries.find(GetTypeIndex<T>()) == m_Registries.end()) {
                RegisterComponent<T>();
            }

            std::dynamic_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeIndex<T>()])->Add(index, component);
        }

        template<typename T>
        Error::Result<std::shared_ptr<T>, ComponentError> GetComponent(size_t index) {
            if (m_Registries.find(GetTypeIndex<T>()) == m_Registries.end()) {
                return ComponentError::NotRegistered;
            }

            auto result = std::dynamic_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeIndex<T>()])->Get(index);

            if (result.IsError()) {
                if (result.UnwrapError() == ComponentRegistryError::NotFound) {
                    return ComponentError::NotFound;
                }
            }

            return result.Unwrap();
        }

        template<typename T>
        bool HasComponent(size_t index) {
            if (m_Registries.find(GetTypeIndex<T>()) == m_Registries.end()) {
                return false;
            }

            return m_Registries[GetTypeIndex<T>()]->Has(index);
        }

        template<typename T>
        void RemoveComponent(size_t index) {
            if (m_Registries.find(GetTypeIndex<T>()) == m_Registries.end()) {
                return;
            }

            m_Registries[GetTypeIndex<T>()]->Remove(index);
        }

        void ClearComponents(size_t index) const;
        void ClearComponents() const;

        template<typename T>
        Error::Result<std::shared_ptr<ComponentRegistry<T>>, ComponentError> QueryComponents() {
            if (m_Registries.find(GetTypeIndex<T>()) == m_Registries.end()) {
                return ComponentError::NotRegistered;
            }

            return std::dynamic_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeIndex<T>()]);
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<IComponentRegistry>> m_Registries;

        template<typename T>
        static std::type_index GetTypeIndex() {
            return std::type_index(typeid(T));
        }
    };
} // namespace Pixf::Core::Entities

#endif // COMPONENTMANAGER_HPP
