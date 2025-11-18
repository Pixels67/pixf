#include "ShaderStore.hpp"

static const auto s_StandardVertShader = R"(
#version 330 core

layout(location = 0) in vec3 aVertPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoords;

uniform mat4 uModel;
uniform mat4 uProj;

void main() {
    gl_Position = vec4(aVertPos, 1.0) * uModel * uProj;
    vTexCoords = aTexCoord;
}
)";

static const auto s_StandardFragShader = R"(
#version 330 core

in vec2 vTexCoords;

out vec4 fragColor;

uniform sampler2D uDiffuseTex;

void main() {
    fragColor = texture(uDiffuseTex, vTexCoords);
}
)";

namespace Pixf::Core::Graphics {
    ShaderStore::ShaderStore() {
        Create(s_StandardVertShader, s_StandardFragShader);
    }

    ShaderHandle ShaderStore::Create(const std::string &vertexSource, const std::string &fragmentSource) {
        auto [idx, slot] = GetSlot();

        slot.shader = Gl::Shader::Create(vertexSource, fragmentSource);
        slot.active = true;

        return {.id = idx, .version = slot.version};
    }

    void ShaderStore::Destroy(const ShaderHandle handle) {
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

    ShaderHandle ShaderStore::GetStandardShader() { return {.id = 0, .version = 0}; }

    std::pair<uint32_t, ShaderStore::Slot &> ShaderStore::GetSlot() {
        for (uint32_t i = 0; i < m_Shaders.size(); i++) {
            if (m_Shaders[i].active) {
                continue;
            }

            return {i, m_Shaders[i]};
        }

        m_Shaders.emplace_back();
        return {m_Shaders.size() - 1, m_Shaders.back()};
    }
} // namespace Pixf::Core::Graphics
