#include "AssetManager.hpp"

#include "Files/Audio.hpp"
#include "Files/File.hpp"
#include "Files/Image.hpp"
#include "Files/Model.hpp"

namespace Pixf::Core::Files::Assets {
    AssetManager::AssetManager(const std::string &assetsDirectory) : m_AssetsDirectory(assetsDirectory) {
        RegisterAssetPaths();
    }

    ModelAssetHandle AssetManager::ImportModel(const std::string &filepath, Resources &resources) {
        std::vector<MeshData> meshes = LoadModelMeshes(filepath);
        std::vector<MaterialData> materials = LoadModelMaterials(filepath);

        std::vector<Graphics::MaterialHandle> outputMaterials;

        outputMaterials.reserve(materials.size());
        for (auto &[diffusePath, specularPath, diffuseColor, specularColor]: materials) {
            Graphics::Material material;
            material.SetShader(resources.shaderStore.GetStandardShader());

            if (diffusePath) {
                const auto diff = ImportTexture2D(diffusePath.value(), {}, resources);
                material.SetDiffuseTexture(GetTexture2D(diff));
            }

            if (specularPath) {
                const auto spec = ImportTexture2D(specularPath.value(), {}, resources);
                material.SetSpecularTexture(GetTexture2D(spec));
            }

            material.SetDiffuse(diffuseColor);
            material.SetSpecular(specularColor);

            outputMaterials.push_back(resources.materialStore.Create(std::move(material)));
        }

        Graphics::Model model;
        for (auto &[data, materialIndex]: meshes) {
            model.elements.push_back(
                    {resources.meshStore.Create(Graphics::Mesh(data)), outputMaterials[materialIndex]});
        }

        const auto uuid = PathToUuid(filepath);
        m_Models[uuid.Hash()] = model;
        return {uuid};
    }

    Texture2DAssetHandle AssetManager::ImportTexture2D(const std::string &filepath,
                                             const Graphics::Gl::Texture2D::Config config,
                                             Resources &resources) {
        const auto image = LoadImage(filepath);
        const auto texture = resources.textureStore.Create(image, config);

        const auto uuid = PathToUuid(filepath);
        m_Textures[uuid.Hash()] = texture;
        return {uuid};
    }

    ClipAssetHandle AssetManager::ImportClip(const std::string &filepath, Resources &resources) {
        auto &&audio = LoadAudioFile(filepath);
        const Audio::ClipHandle clip = resources.clipStore.Create(std::move(audio));

        const auto uuid = PathToUuid(filepath);
        m_Clips[uuid.Hash()] = clip;
        return {uuid};
    }

    Graphics::Model AssetManager::GetModel(const ModelAssetHandle &model) {
        if (!m_Models.contains(model.uuid.Hash())) {
            throw AssetError("Invalid UUID used to retrieve model: " + model.uuid.ToString());
        }

        return m_Models[model.uuid.Hash()];
    }

    Graphics::Texture2DHandle AssetManager::GetTexture2D(const Texture2DAssetHandle &texture) {
        if (!m_Textures.contains(texture.uuid.Hash())) {
            throw AssetError("Invalid UUID used to retrieve texture: " + texture.uuid.ToString());
        }

        return m_Textures[texture.uuid.Hash()];
    }

    Audio::ClipHandle AssetManager::GetClip(const ClipAssetHandle &clip) {
        if (!m_Clips.contains(clip.uuid.Hash())) {
            throw AssetError("Invalid UUID used to retrieve clip: " + clip.uuid.ToString());
        }

        return m_Clips[clip.uuid.Hash()];
    }

    Uuid::Uuid AssetManager::PathToUuid(const std::string &filepath) {
        if (!FileExists(filepath + ".meta")) {
            GenerateMetaFile(filepath);
        }

        const Uuid::Uuid uuid = Uuid::Uuid::FromString(ReadFile(filepath + ".meta")).value();
        return uuid;
    }

    std::optional<std::string> AssetManager::UuidToPath(const Uuid::Uuid &uuid) const {
        if (!m_AssetPaths.contains(uuid.Hash())) {
            return std::nullopt;
        }

        return m_AssetPaths.at(uuid.Hash());
    }

    void AssetManager::GenerateMetaFile(const std::string &filepath) {
        const Uuid::Uuid uuid = Uuid::Uuid::FromName(filepath, Uuid::Uuid::UrlNamespace());
        WriteFile(filepath + ".meta", uuid.ToString());
        RegisterAssetPaths();
    }

    void AssetManager::RegisterAssetPaths() {
        for (auto path: GetFilesInDirectory(m_AssetsDirectory, ".meta", true)) {
            path.erase(path.find_last_of('.'));
            m_AssetPaths[PathToUuid(path).Hash()] = path;
        }
    }
} // namespace Pixf::Core::Files::Assets
