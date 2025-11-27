#include "MeshStore.hpp"

namespace Pixf::Core::Graphics {
    MeshHandle MeshStore::Create(Mesh &&mesh) {
        auto [idx, slot] = GetSlot();

        slot.mesh = std::move(mesh);
        slot.active = true;

        return {.id = idx, .version = slot.version};
    }

    void MeshStore::Destroy(const MeshHandle handle) {
        if (handle.id >= m_Meshes.size()) {
            PIXF_CORE_LOG_ERROR("Failed to unload mesh: ID {} not found", handle.id);
            return;
        }

        auto &[mesh, version, active] = m_Meshes.at(handle.id);

        if (version != handle.version || !active) {
            PIXF_CORE_LOG_ERROR("Failed to unload mesh: mesh {} already unloaded", handle.id);
            return;
        }

        mesh.Clear();
        active = false;
        version++;
    }

    Mesh &MeshStore::Get(const MeshHandle handle) {
        if (handle.id >= m_Meshes.size()) {
            throw MeshStoreError("Failed to retrieve mesh: ID " + std::to_string(handle.id) + " not found");
        }

        auto &[mesh, version, active] = m_Meshes.at(handle.id);

        if (version != handle.version || !active) {
            throw MeshStoreError("Failed to retrieve mesh: mesh " + std::to_string(handle.id) + " is unloaded");
        }

        return mesh;
    }

    std::pair<uint32_t, MeshStore::Slot &> MeshStore::GetSlot() {
        for (uint32_t i = 0; i < m_Meshes.size(); i++) {
            if (m_Meshes[i].active) {
                continue;
            }

            return {i, m_Meshes[i]};
        }

        m_Meshes.emplace_back();
        return {m_Meshes.size() - 1, m_Meshes.back()};
    }
}
