#include "ComponentManager.hpp"

#include <unordered_map>

#include "ComponentRegistry.hpp"

namespace Pixf::Core::Entities {
    void ComponentManager::AddComponent(const size_t index, const std::string &typeName) {
        const uint64_t typeId = HashString(typeName.c_str());
        if (!m_Registries.contains(typeId)) {
            return;
        }

        m_Registries[typeId]->Add(index);
    }

    void ComponentManager::ClearComponents(const size_t index) const {
        for (const auto &[_, registry]: m_Registries) {
            registry->Remove(index);
        }
    }

    void ComponentManager::Clear() const {
        for (const auto &[_, registry]: m_Registries) {
            registry->Clear();
        }
    }

    std::vector<std::string> ComponentManager::GetComponentTypeNames() const {
        std::vector<std::string> names;
        for (const auto &[_, registry]: m_Registries) {
            names.push_back(registry->GetTypeName());
        }

        return names;
    }
} // namespace Pixf::Core::Entities
