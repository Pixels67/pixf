#include "ShaderStore.hpp"

namespace Pixf::Core::Graphics {
    ShaderHandle ShaderStore::Load(const std::string &vertexSource, const std::string &fragmentSource) {
        auto [idx, slot] = GetSlot();

        slot.shader = Gl::Shader::Create(vertexSource, fragmentSource);
        slot.active = true;

        return {.id = idx, .version = slot.version};
    }

    void ShaderStore::Unload(const ShaderHandle handle) {
        if (handle.id >= m_Shaders.size()) {
            PIXF_CORE_LOG_ERROR("Failed to unload shader: ID {} not found", handle.id);
            return;
        }

        auto &[shader, version, active] = m_Shaders.at(handle.id);

        if (version != handle.version || !active) {
            PIXF_CORE_LOG_ERROR("Failed to unload shader: Shader {} already unloaded", handle.id);
            return;
        }

        shader.Clear();
        active = false;
        version++;
    }

    Gl::Shader &ShaderStore::Get(const ShaderHandle handle) {
        if (handle.id >= m_Shaders.size()) {
            throw ShaderStoreError("Failed to retrieve shader: ID " + std::to_string(handle.id) + " not found");
        }

        auto &[shader, version, active] = m_Shaders.at(handle.id);

        if (version != handle.version || !active) {
            throw ShaderStoreError("Failed to retrieve shader: Shader " + std::to_string(handle.id) + " is unloaded");
        }

        return shader;
    }

    std::pair<uint32_t, ShaderStore::Slot &> ShaderStore::GetSlot() {
        for (uint32_t i = 0; i < m_Shaders.size(); i++) {
            if (m_Shaders[i].active) {
                continue;
            }

            return {i, m_Shaders[i]};
        }

        m_Shaders.resize(m_Shaders.size() + 1);
        return {m_Shaders.size() - 1, m_Shaders.back()};
    }
} // namespace Pixf::Core::Graphics
