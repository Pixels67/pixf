#include "ClipStore.hpp"

namespace Pixf::Core::Audio::Ma {
    ClipHandle ClipStore::Create(Clip &&clip) {
        auto [idx, slot] = GetSlot();

        slot.clip = std::move(clip);
        slot.active = true;

        return {.id = idx, .version = slot.version};
    }

    void ClipStore::Destroy(const ClipHandle handle) {
        if (handle.id >= m_Clips.size()) {
            PIXF_CORE_LOG_ERROR("Failed to unload clip: ID {} not found", handle.id);
            return;
        }

        auto &[clip, version, active] = m_Clips.at(handle.id);

        if (version != handle.version || !active) {
            PIXF_CORE_LOG_ERROR("Failed to unload clip: clip {} already unloaded", handle.id);
            return;
        }

        clip.Clear();
        active = false;
        version++;
    }

    Clip &ClipStore::Get(const ClipHandle handle) {
        if (handle.id >= m_Clips.size()) {
            throw ClipStoreError("Failed to retrieve clip: ID " + std::to_string(handle.id) + " not found");
        }

        auto &[clip, version, active] = m_Clips.at(handle.id);

        if (version != handle.version || !active) {
            throw ClipStoreError("Failed to retrieve clip: clip " + std::to_string(handle.id) + " is unloaded");
        }

        return clip;
    }

    std::pair<uint32_t, ClipStore::Slot &> ClipStore::GetSlot() {
        for (uint32_t i = 0; i < m_Clips.size(); i++) {
            if (m_Clips[i].active) {
                continue;
            }

            return {i, m_Clips[i]};
        }

        m_Clips.emplace_back();
        return {m_Clips.size() - 1, m_Clips.back()};
    }
} // namespace Pixf::Core::Audio::Ma
