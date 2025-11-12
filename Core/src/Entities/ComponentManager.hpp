#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#include <typeindex>
#include <unordered_map>

#include "ComponentRegistry.hpp"
#include "Error/Result.hpp"
#include "Input/InputManager.hpp"

namespace Pixf::Core::Entities {
    enum class ComponentError : uint8_t {
        None = 0,
        NotFound,
        NotRegistered,
    };

    class ComponentManager final : Serialization::Serializable {
    public:
        ComponentManager() = default;

        ComponentManager(const ComponentManager &) = default;
        ComponentManager(ComponentManager &&) = delete;
        ComponentManager &operator=(const ComponentManager &) = default;
        ComponentManager &operator=(ComponentManager &&) = delete;

        ~ComponentManager() override = default;

        template<typename T>
        void RegisterComponent() {
            std::shared_ptr<ComponentRegistry<T>> ptr = std::make_shared<ComponentRegistry<T>>();

            m_Registries[GetTypeIndex<T>()] = std::static_pointer_cast<IComponentRegistry>(ptr);
            if (ptr->GetTypeName()) {
                const std::string name = ptr->GetTypeName();
                m_Types.insert({name, GetTypeIndex<T>()});
            }
        }

        template<typename T>
        void AddComponent(size_t index) {
            if (!m_Registries.contains(GetTypeIndex<T>())) {
                RegisterComponent<T>();
            }

            m_Registries[GetTypeIndex<T>()]->Add(index);
        }

        template<typename T>
        void AddComponent(size_t index, T component) {
            if (!m_Registries.contains(GetTypeIndex<T>())) {
                RegisterComponent<T>();
            }

            std::dynamic_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeIndex<T>()])->Add(index, component);
        }

        template<typename T>
        Error::Result<std::shared_ptr<T>, ComponentError> GetComponent(size_t index) {
            if (!m_Registries.contains(GetTypeIndex<T>())) {
                return ComponentError::NotRegistered;
            }

            auto result = std::dynamic_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeIndex<T>()])->Get(index);

            if (result.IsError()) {
                if (result.UnwrapError() == ComponentRegistryError::NotFound) {
                    return ComponentError::NotFound;
                }

                PIXF_ASSERT(false, "Unknown error");
            }

            return result.Unwrap();
        }

        template<typename T>
        bool HasComponent(size_t index) {
            if (!m_Registries.contains(GetTypeIndex<T>())) {
                return false;
            }

            return m_Registries[GetTypeIndex<T>()]->Has(index);
        }

        template<typename T>
        void RemoveComponent(size_t index) {
            if (!m_Registries.contains(GetTypeIndex<T>())) {
                return;
            }

            m_Registries[GetTypeIndex<T>()]->Remove(index);
        }

        void ClearComponents(size_t index) const;
        void Clear() const;

        template<typename T>
        Error::Result<std::shared_ptr<ComponentRegistry<T>>, ComponentError> QueryComponents() {
            if (!m_Registries.contains(GetTypeIndex<T>())) {
                return ComponentError::NotRegistered;
            }

            return std::dynamic_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeIndex<T>()]);
        }

        Json::object Serialize() override {
            Json::object json;

            for (auto &[id, registry]: m_Registries) {
                json[std::string(registry->GetTypeName())] =
                        std::dynamic_pointer_cast<Serializable>(registry)->Serialize();
            }

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {
            for (auto &[key, value]: json) {
                if (!m_Types.contains(key)) {
                    PIXF_LOG_ERROR("Component ", key, " not registered!");
                    continue;
                }

                m_Registries[m_Types.at(key)]->Deserialize(value.as_object(), assetManager);
            }
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<IComponentRegistry>> m_Registries;
        std::unordered_map<std::string, std::type_index> m_Types;

        template<typename T>
        static std::type_index GetTypeIndex() {
            return std::type_index(typeid(T));
        }
    };
} // namespace Pixf::Core::Entities

#endif // COMPONENTMANAGER_HPP
