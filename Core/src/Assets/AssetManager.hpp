#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <boost/json/value.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include "Error/Result.hpp"
#include "Graphics/Gl/Shader.hpp"
#include "Graphics/Gl/Texture2D.hpp"
#include "Graphics/Mesh.hpp"

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
    mat4 invView = inverse(uView);
    vViewPos = invView[3].xyz;
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
    vec3 normal = normalize(vNormal);
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

    fragColor = ambient + diff + spec;
}
)";

namespace Pixf::Core::Graphics {
    class Model;
    class Material;
} // namespace Pixf::Core::Graphics

namespace Pixf::Core::Audio {
    struct AudioClip;
}

namespace Pixf::Core::Assets {
    enum class AssetType : uint8_t {
        None = 0,
        Mesh,
        Shader,
        Texture2D,
        Material,
        Model,
        AudioClip,
    };

    inline std::string ToString(const AssetType assetType) {
        switch (assetType) {
            case AssetType::None:
                return "None";
            case AssetType::Mesh:
                return "Mesh";
            case AssetType::Shader:
                return "Shader";
            case AssetType::Texture2D:
                return "Texture2D";
            case AssetType::Material:
                return "Material";
            case AssetType::Model:
                return "Model";
            case AssetType::AudioClip:
                return "AudioClip";
            default:
                return "Unknown";
        }
    }

    class AssetManager;

    struct PIXF_API AssetHandle {
        friend class AssetManager;
        AssetHandle() = default;

        uuids::uuid GetUuid() const { return uuid; }
        AssetType GetType() const { return type; }
        AssetManager *GetAssetManager() const { return assetManager; }

    private:
        uuids::uuid uuid;
        AssetType type = AssetType::None;
        AssetManager *assetManager = nullptr;

        explicit AssetHandle(AssetManager &assetManager, const uuids::uuid uuid, const AssetType type) :
            uuid(uuid), type(type), assetManager(&assetManager) {}
    };

    enum class AssetError : uint8_t {
        None = 0,
        NotFound,
        FailedToLoad,
        FailedToParseMetaFile,
        MismatchedType,
        NotRegistered,
    };

    class PIXF_API AssetManager {
    public:
        explicit AssetManager(const std::string &assetsPath = "Assets/");

        AssetManager(const AssetManager &) = delete;
        AssetManager(AssetManager &&) = default;
        AssetManager &operator=(const AssetManager &) = delete;
        AssetManager &operator=(AssetManager &&) = default;

        ~AssetManager();

        Error::Result<AssetHandle, AssetError> ImportTexture2D(const std::string &path,
                                                               Graphics::Gl::TextureConfig config = {});
        Error::Result<AssetHandle, AssetError> ImportModel(const std::string &path);
        Error::Result<AssetHandle, AssetError> ImportAudioClip(const std::string &path);

        AssetHandle CreateShader(const std::string &vertSrc = g_DefaultVertShader,
                                 const std::string &fragSrc = g_DefaultFragShader);
        AssetHandle CreateMaterial();
        AssetHandle CreateMaterial(const AssetHandle &shader);

        Error::Result<std::shared_ptr<Graphics::Gl::Shader>, AssetError> GetShader(const AssetHandle &handle);
        Error::Result<std::shared_ptr<Graphics::Gl::Texture2D>, AssetError> GetTexture2D(const AssetHandle &handle);
        Error::Result<std::shared_ptr<Graphics::Material>, AssetError> GetMaterial(const AssetHandle &handle);
        Error::Result<std::shared_ptr<Graphics::Model>, AssetError> GetModel(const AssetHandle &handle);
        Error::Result<std::shared_ptr<Audio::AudioClip>, AssetError> GetAudioClip(const AssetHandle &handle);

        void DeleteShader(const AssetHandle &handle);
        void DeleteTexture2D(const AssetHandle &handle);
        void DeleteMaterial(const AssetHandle &handle);
        void DeleteModel(const AssetHandle &handle);
        void DeleteAudioClip(const AssetHandle &handle);

        AssetHandle CreateMesh(const std::vector<Graphics::Vertex> &vertices, std::vector<unsigned int> indices = {});

        Error::Result<std::shared_ptr<Graphics::Mesh>, AssetError> GetMesh(const AssetHandle &handle);

        void DeleteMesh(const AssetHandle &handle);

        Error::Result<std::string, AssetError> GetAssetPath(const AssetHandle &handle);
        Error::Result<std::string, AssetError> GetAssetPath(const uuids::uuid &uuid);

        void DeleteAllAudioClips();

    private:
        uuids::name_generator m_UuidGenerator = uuids::name_generator(uuids::ns::url());
        uuids::random_generator m_RandomUuidGenerator = {};

        std::unordered_map<uuids::uuid, std::shared_ptr<Graphics::Mesh>> m_Meshes;
        std::unordered_map<uuids::uuid, std::shared_ptr<Graphics::Gl::Shader>> m_Shaders;
        std::unordered_map<uuids::uuid, std::shared_ptr<Graphics::Gl::Texture2D>> m_Textures2D;
        std::unordered_map<uuids::uuid, std::shared_ptr<Graphics::Material>> m_Materials;
        std::unordered_map<uuids::uuid, std::shared_ptr<Graphics::Model>> m_Models;
        std::unordered_map<uuids::uuid, std::shared_ptr<Audio::AudioClip>> m_AudioClips;

        std::unordered_map<std::string, uuids::uuid> m_Texture2DPaths;
        std::unordered_map<std::string, uuids::uuid> m_ModelPaths;
        std::unordered_map<std::string, uuids::uuid> m_AudioClipPaths;

        std::unordered_map<uuids::uuid, std::string> m_AssetPaths;

        Error::Result<uuids::uuid, AssetError> GetUuid(const std::string &path, AssetType type) const;
    };
} // namespace Pixf::Core::Assets

#endif // ASSETMANAGER_HPP
