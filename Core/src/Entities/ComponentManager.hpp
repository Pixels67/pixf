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

        template<TypeInformed T>
        void RegisterComponent() {
            std::shared_ptr<ComponentRegistry<T>> ptr = std::make_shared<ComponentRegistry<T>>();

            m_Registries[GetTypeId<T>()] = std::static_pointer_cast<IComponentRegistry>(ptr);
            if (ptr->GetTypeName()) {
                const std::string name = ptr->GetTypeName();
                m_Types.insert({name, GetTypeId<T>()});
                PIXF_LOG_DEBUG("Registered component ", ptr->GetTypeName(), " as ", GetTypeId<T>());
            }
        }

        template<TypeInformed T>
        void AddComponent(size_t index) {
            if (!m_Registries.contains(GetTypeId<T>())) {
                RegisterComponent<T>();
            }

            m_Registries[GetTypeId<T>()]->Add(index);
        }

        template<TypeInformed T>
        void AddComponent(size_t index, T component) {
            if (!m_Registries.contains(GetTypeId<T>())) {
                RegisterComponent<T>();
            }

            std::static_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeId<T>()])->Add(index, component);
        }

        template<TypeInformed T>
        Error::Result<std::shared_ptr<T>, ComponentError> GetComponent(size_t index) {
            if (!m_Registries.contains(GetTypeId<T>())) {
                return ComponentError::NotRegistered;
            }

            auto result = std::static_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeId<T>()])->Get(index);

            if (result.IsError()) {
                if (result.UnwrapError() == ComponentRegistryError::NotFound) {
                    return ComponentError::NotFound;
                }

                PIXF_ASSERT(false, "Unknown error");
            }

            return result.Unwrap();
        }

        template<TypeInformed T>
        bool HasComponent(size_t index) {
            if (!m_Registries.contains(GetTypeId<T>())) {
                return false;
            }

            return m_Registries[GetTypeId<T>()]->Has(index);
        }

        bool HasComponent(const size_t index, const uint64_t typeId) {
            if (!m_Registries.contains(typeId)) {
                return false;
            }

            return m_Registries[typeId]->Has(index);
        }

        template<TypeInformed T>
        void RemoveComponent(size_t index) {
            if (!m_Registries.contains(GetTypeId<T>())) {
                return;
            }

            m_Registries[GetTypeId<T>()]->Remove(index);
        }

        void ClearComponents(size_t index) const;
        void Clear() const;

        template<TypeInformed T>
        Error::Result<std::shared_ptr<ComponentRegistry<T>>, ComponentError> QueryComponents() {
            if (!m_Registries.contains(GetTypeId<T>())) {
                return ComponentError::NotRegistered;
            }

            return std::static_pointer_cast<ComponentRegistry<T>>(m_Registries[GetTypeId<T>()]);
        }

        Json::object Serialize(bool editorMode = false) override {
            Json::object json;

            for (auto &[id, registry]: m_Registries) {
                json[std::string(registry->GetTypeName())] =
                        std::static_pointer_cast<Serializable>(registry)->Serialize(editorMode);
            }

            return json;
        }

        void Deserialize(const Json::object &json, Assets::AssetManager &assetManager,
                         bool editorMode = false) override {
            for (auto &[key, value]: json) {
                if (!m_Types.contains(key)) {
                    PIXF_LOG_ERROR("Component ", key, " not registered!");
                    continue;
                }

                m_Registries[m_Types.at(key)]->Deserialize(value.as_object(), assetManager, editorMode);
            }
        }

        Json::object SerializeElement(const size_t index, bool editorMode = false) {
            Json::object json;

            for (auto &[id, registry]: m_Registries) {
                if (!HasComponent(index, registry->GetTypeId())) {
                    continue;
                }

                json[std::string(registry->GetTypeName())] = registry->SerializeElement(index, editorMode);
            }

            return json;
        }

        void DeserializeElement(const Json::object &json, Assets::AssetManager &assetManager, const size_t index, bool editorMode = false) {
            for (auto &[key, value]: json) {
                if (!m_Types.contains(key)) {
                    PIXF_LOG_ERROR("Component ", key, " not registered!");
                    continue;
                }

                m_Registries[m_Types.at(key)]->DeserializeElement(value.as_object(), assetManager, index, editorMode);
            }
        }

    private:
        std::unordered_map<uint64_t, std::shared_ptr<IComponentRegistry>> m_Registries;
        std::unordered_map<std::string, uint64_t> m_Types;

        template<TypeInformed T>
        static uint64_t GetTypeId() {
            return T::GetTypeId();
        }
    };
} // namespace Pixf::Core::Entities

#endif // COMPONENTMANAGER_HPP
