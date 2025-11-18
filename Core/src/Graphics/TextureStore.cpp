#include "TextureStore.hpp"

#include "Debug/Logger.hpp"

namespace Pixf::Core::Graphics {
    Texture2DHandle TextureStore::Create(const ImageData &image, const Gl::Texture2D::Config config) {
        auto [idx, slot] = GetSlot();

        slot.texture2D = Gl::Texture2D::Create(image, config);
        slot.active = true;

        return {.id = idx, .version = slot.version};
    }
    
    void TextureStore::Destroy(const Texture2DHandle handle) {
        if (handle.id >= m_Textures2D.size()) {
            PIXF_CORE_LOG_ERROR("Failed to unload 2D texture: ID {} not found", handle.id);
            return;
        }

        auto &[texture, version, active] = m_Textures2D.at(handle.id);

        if (version != handle.version || !active) {
            PIXF_CORE_LOG_ERROR("Failed to unload 2d texture: texture {} already unloaded", handle.id);
            return;
        }

        texture.Clear();
        active = false;
        version++;
    }
    
    Gl::Texture2D &TextureStore::Get(const Texture2DHandle handle) {
        if (handle.id >= m_Textures2D.size()) {
            throw TextureStoreError("Failed to retrieve shader: ID " + std::to_string(handle.id) + " not found");
        }

        auto &[shader, version, active] = m_Textures2D.at(handle.id);

        if (version != handle.version || !active) {
            throw TextureStoreError("Failed to retrieve shader: Shader " + std::to_string(handle.id) + " is unloaded");
        }

        return shader;
    }
    
    std::pair<uint32_t, TextureStore::Slot &> TextureStore::GetSlot() {
        for (uint32_t i = 0; i < m_Textures2D.size(); i++) {
            if (m_Textures2D[i].active) {
                continue;
            }

            return {i, m_Textures2D[i]};
        }

        m_Textures2D.emplace_back();
        return {m_Textures2D.size() - 1, m_Textures2D.back()};
    }
} // namespace Pixf::Core::Graphics
