#include "ComponentManager.hpp"

#include <unordered_map>

#include "ComponentRegistry.hpp"

namespace Pixf::Core::Entities {
    void ComponentManager::ClearComponents(const size_t index) const {
        for (const auto &[_, registry]: m_Registries) {
            registry->Remove(index);
        }
    }

    void ComponentManager::ClearComponents() const {
        for (const auto &[_, registry]: m_Registries) {
            registry->Clear();
        }
    }
} // namespace Pixf::Core::Entities
