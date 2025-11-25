#include "ShaderStore.hpp"

const auto s_StandardVertShader = R"(
#version 330 core

layout(location = 0) in vec3 aVertPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 vWorldPos;
out vec3 vViewPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    gl_Position = vec4(aVertPos, 1.0) * uModel * uView * uProj;
    vWorldPos = (vec4(aVertPos, 1.0) * uModel).xyz;
    mat4 invView = inverse(uView);
    vViewPos = invView[3].xyz;
    vNormal = normalize(aNormal) * mat3(transpose(inverse(uModel)));
    vTexCoords = aTexCoords;
}
)";

const auto s_StandardFragShader = R"(
#version 330 core

#define MAX_DIRECTIONAL_LIGHTS 8
#define MAX_POINT_LIGHTS 16

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;

    float linearFalloff;
    float quadraticFalloff;
};

in vec3 vWorldPos;
in vec3 vViewPos;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fragColor;

uniform AmbientLight uAmbientLight;
uniform DirectionalLight uDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int uDirectionalLightCount;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform int uPointLightCount;

uniform vec3 uDiffuse;
uniform sampler2D uDiffuseTex;

uniform vec3 uSpecular;
uniform sampler2D uSpecularTex;

uniform float uSpecularStrength;
uniform float uShininess;

float diffuse(vec3 normal, vec3 lightDir) {
    return max(dot(normalize(normal), -normalize(lightDir)), 0.0);
}

float specular(vec3 worldPos, vec3 viewPos, vec3 lightDir, vec3 normal, float specularStrength, float shininess) {
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularStrength * spec;
}

void main() {
    vec3 normal = normalize(vNormal);
    vec4 diffuseColor = vec4(uDiffuse, 1.0) * texture(uDiffuseTex, vTexCoords);
    vec4 specularColor = vec4(uSpecular, 1.0) * texture(uSpecularTex, vTexCoords);

    vec4 ambient = uAmbientLight.intensity * vec4(uAmbientLight.color, 1.0) * diffuseColor;
    vec4 diff = vec4(0.0);
    vec4 spec = vec4(0.0);

    for (int i = 0; i < min(uDirectionalLightCount, MAX_DIRECTIONAL_LIGHTS); i++) {
        DirectionalLight light = uDirectionalLights[i];
        diff += vec4(light.intensity * light.color * diffuse(normal, light.direction), 1.0) * diffuseColor;
        spec += vec4(light.intensity * light.color, 1.0) * specular(vWorldPos, vViewPos, light.direction, normal, uSpecularStrength, uShininess) * specularColor;
    }

    for (int i = 0; i < min(uPointLightCount, MAX_POINT_LIGHTS); i++) {
        PointLight light = uPointLights[i];
        float distance = length(vWorldPos - light.position);
        float attenuation = 1.0 / (1.0 + light.linearFalloff * distance + light.quadraticFalloff * distance * distance);
        attenuation *= light.intensity;

        vec3 lightDir = normalize(vWorldPos - light.position);

        diff += vec4(attenuation * light.color * diffuse(normal, lightDir), 1.0) * diffuseColor;
        spec += vec4(attenuation * light.color, 1.0) * specular(vWorldPos, vViewPos, lightDir, normal, uSpecularStrength, uShininess) * specularColor;
    }

    //fragColor = ambient + diff + spec;
    fragColor = diffuseColor;
}
)";

namespace Pixf::Core::Graphics {
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

    ShaderHandle ShaderStore::GetStandardShader() {
        if (m_Shaders.size() == 0) {
            m_Shaders.emplace_back();
        }

        if (!m_Shaders[0].active) {
            m_Shaders[0].shader = Gl::Shader::Create(s_StandardVertShader, s_StandardFragShader);
            m_Shaders[0].active = true;
        }

        return {.id = 0, .version = 0};
    }

    std::pair<uint32_t, ShaderStore::Slot &> ShaderStore::GetSlot() {
        for (uint32_t i = 1; i < m_Shaders.size(); i++) {
            if (m_Shaders[i].active) {
                continue;
            }

            return {i, m_Shaders[i]};
        }

        m_Shaders.emplace_back();
        return {m_Shaders.size() - 1, m_Shaders.back()};
    }
} // namespace Pixf::Core::Graphics
