#include "MaterialStore.hpp"

namespace Pixf::Core::Graphics {
    MaterialHandle MaterialStore::Create(Material &&material) {
        auto [idx, slot] = GetSlot();

        slot.material = std::move(material);
        slot.active = true;

        return {.id = idx, .version = slot.version};
    }
    
    void MaterialStore::Destroy(const MaterialHandle handle) {
        if (handle.id >= m_Materials.size()) {
            PIXF_CORE_LOG_ERROR("Failed to unload material: ID {} not found", handle.id);
            return;
        }

        auto &[material, version, active] = m_Materials.at(handle.id);

        if (version != handle.version || !active) {
            PIXF_CORE_LOG_ERROR("Failed to unload material: material {} already unloaded", handle.id);
            return;
        }

        material.Clear();
        active = false;
        version++;
    }
    
    Material &MaterialStore::Get(const MaterialHandle handle) {
        if (handle.id >= m_Materials.size()) {
            throw MaterialStoreError("Failed to retrieve material: ID " + std::to_string(handle.id) + " not found");
        }

        auto &[material, version, active] = m_Materials.at(handle.id);

        if (version != handle.version || !active) {
            throw MaterialStoreError("Failed to retrieve material: material " + std::to_string(handle.id) + " is unloaded");
        }

        return material;
    }
    
    std::pair<uint32_t, MaterialStore::Slot &> MaterialStore::GetSlot() {
        for (uint32_t i = 0; i < m_Materials.size(); i++) {
            if (m_Materials[i].active) {
                continue;
            }

            return {i, m_Materials[i]};
        }

        m_Materials.emplace_back();
        return {m_Materials.size() - 1, m_Materials.back()};
    }
} // namespace Pixf::Core::Graphics
