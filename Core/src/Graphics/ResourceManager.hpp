#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Error/Result.hpp"
#include "Gl/Shader.hpp"
#include "Gl/Texture2D.hpp"
#include "Mesh.hpp"

const auto g_DefaultVertShader = R"(
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
    gl_Position = uProj * uView * uModel * vec4(aVertPos, 1.0);
    vWorldPos = (uModel * vec4(aVertPos, 1.0)).xyz;
    vViewPos = (uView * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    vNormal = mat3(transpose(inverse(uModel))) * normalize(aNormal);
    vTexCoords = aTexCoords;
}
)";

const auto g_DefaultFragShader = R"(
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

uniform vec4 uDiffuse;
uniform sampler2D uDiffuseTex;
uniform bool uHasDiffuseTex;

uniform vec4 uSpecular;
uniform sampler2D uSpecularTex;
uniform bool uHasSpecularTex;

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
    vec4 diffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
    if (uHasDiffuseTex) {
        diffuseColor = uDiffuse * texture(uDiffuseTex, vTexCoords);
    } else {
        diffuseColor = uDiffuse;
    }

    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);
    if (uHasSpecularTex) {
        specularColor = uSpecular * texture(uSpecularTex, vTexCoords);
    } else {
        specularColor = uSpecular;
    }

    vec4 ambient = uAmbientLight.intensity * vec4(uAmbientLight.color, 1.0) * diffuseColor;
    vec4 diff = vec4(0.0);
    vec4 spec = vec4(0.0);

    for (int i = 0; i < min(uDirectionalLightCount, MAX_DIRECTIONAL_LIGHTS); i++) {
        DirectionalLight light = uDirectionalLights[i];
        diff += vec4(light.intensity * light.color * diffuse(vNormal, light.direction), 1.0) * diffuseColor;
        spec += vec4(light.intensity * light.color, 1.0) * specular(vWorldPos, vViewPos, light.direction, vNormal, uSpecularStrength, uShininess) * specularColor;
    }

    for (int i = 0; i < min(uPointLightCount, MAX_POINT_LIGHTS); i++) {
        PointLight light = uPointLights[i];
        float distance = length(vWorldPos - light.position);
        float attenuation = 1.0 / (1.0 + light.linearFalloff * distance + light.quadraticFalloff * distance * distance);
        attenuation *= light.intensity;

        vec3 lightDir = normalize(vWorldPos - light.position);

        diff += vec4(attenuation * light.color * diffuse(vNormal, lightDir), 1.0) * diffuseColor;
        spec += vec4(attenuation * light.color, 1.0) * specular(vWorldPos, vViewPos, lightDir, vNormal, uSpecularStrength, uShininess) * specularColor;
    }

    fragColor = ambient + diff + spec;
}
)";

namespace Pixf::Core::Graphics {
    class Model;
    class Material;
    struct MeshHandle {
        friend class ResourceManager;

        MeshHandle() = default;

    private:
        unsigned int id = 0;

        explicit MeshHandle(const unsigned int id) : id(id) {}
    };

    struct ShaderHandle {
        friend class ResourceManager;

        ShaderHandle() = default;

    private:
        unsigned int id = 0;

        explicit ShaderHandle(const unsigned int id) : id(id) {}
    };

    struct Texture2DHandle {
        friend class ResourceManager;

        Texture2DHandle() = default;

    private:
        unsigned int id = 0;

        explicit Texture2DHandle(const unsigned int id) : id(id) {}
    };

    struct MaterialHandle {
        friend class ResourceManager;

        MaterialHandle() = default;

    private:
        unsigned int id = 0;

        explicit MaterialHandle(const unsigned int id) : id(id) {}
    };

    struct ModelHandle {
        friend class ResourceManager;

        ModelHandle() = default;

    private:
        unsigned int id = 0;

        explicit ModelHandle(const unsigned int id) : id(id) {}
    };

    enum class ResourceError : uint8_t {
        None = 0,
        NotFound,
        FailedToLoad,
    };

    class ResourceManager {
    public:
        ResourceManager() = default;

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager(ResourceManager &&) = default;
        ResourceManager &operator=(const ResourceManager &) = delete;
        ResourceManager &operator=(ResourceManager &&) = default;

        ~ResourceManager();

        MeshHandle CreateMesh(const std::vector<Vertex> &vertices, std::vector<unsigned int> indices = {});
        ShaderHandle CreateShader(const std::string &vertSrc = g_DefaultVertShader,
                                  const std::string &fragSrc = g_DefaultFragShader);
        Error::Result<Texture2DHandle, ResourceError> ImportTexture2D(const std::string &path,
                                                                      Gl::TextureConfig config = {});
        MaterialHandle CreateMaterial();
        MaterialHandle CreateMaterial(ShaderHandle shader);
        ModelHandle CreateModel(const std::vector<MeshHandle> &meshes, const std::vector<MaterialHandle> &materials);
        Error::Result<ModelHandle, ResourceError> ImportModel(const std::string &path);

        Error::Result<std::shared_ptr<Mesh>, ResourceError> GetMesh(MeshHandle handle);
        Error::Result<std::shared_ptr<Gl::Shader>, ResourceError> GetShader(ShaderHandle handle);
        Error::Result<std::shared_ptr<Gl::Texture2D>, ResourceError> GetTexture2D(Texture2DHandle handle);
        Error::Result<std::shared_ptr<Material>, ResourceError> GetMaterial(MaterialHandle handle);
        Error::Result<std::shared_ptr<Model>, ResourceError> GetModel(ModelHandle handle);

        void DeleteMesh(MeshHandle handle);
        void DeleteShader(ShaderHandle handle);
        void DeleteTexture2D(Texture2DHandle handle);
        void DeleteMaterial(MaterialHandle handle);
        void DeleteModel(ModelHandle handle);

    private:
        std::unordered_map<unsigned int, std::shared_ptr<Mesh>> m_Meshes;
        std::unordered_map<unsigned int, std::shared_ptr<Gl::Shader>> m_Shaders;
        std::unordered_map<unsigned int, std::shared_ptr<Gl::Texture2D>> m_Textures2D;
        std::unordered_map<unsigned int, std::shared_ptr<Material>> m_Materials;
        std::unordered_map<unsigned int, std::shared_ptr<Model>> m_Models;

        std::unordered_map<std::string, unsigned int> m_Texture2DPaths;

        unsigned int m_MeshCounter = 0;
        unsigned int m_ShaderCounter = 0;
        unsigned int m_Texture2DCounter = 0;
        unsigned int m_MaterialCounter = 0;
        unsigned int m_ModelCounter = 0;
    };
} // namespace Pixf::Core::Graphics

#endif // RESOURCEMANAGER_HPP
