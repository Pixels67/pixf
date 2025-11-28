#include "AssetManager.hpp"

#include "Files/Audio.hpp"
#include "Files/File.hpp"
#include "Files/Image.hpp"
#include "Files/Model.hpp"
#include "Json/Json.hpp"

namespace Pixf::Core::Files::Assets {
    std::string ToString(const AssetType type) {
        switch (type) {
            case AssetType::None:
                return "None";
            case AssetType::Model:
                return "Model";
            case AssetType::Texture2D:
                return "Texture2D";
            case AssetType::Clip:
                return "Clip";
            default:
                return "None";
        }
    }

    AssetType FromString(const std::string &str) {
        if (str == "None") {
            return AssetType::None;
        }

        if (str == "Model") {
            return AssetType::Model;
        }

        if (str == "Texture2D") {
            return AssetType::Texture2D;
        }

        if (str == "Clip") {
            return AssetType::Clip;
        }

        return AssetType::None;
    }

    AssetManager::AssetManager(const std::string &assetsDirectory) : m_AssetsDirectory(assetsDirectory) {
        RegisterAssetPaths();
    }

    void AssetManager::ImportAll() {
        for (auto path: GetFilesInDirectory(m_AssetsDirectory, true)) {
            if (!GenerateMetaFile(path)) {
                continue;
            }

            switch (auto [assetUuid, type] = GetAssetMetaData(path + ".meta"); type) {
                case AssetType::Model:
                    ImportModel(path);
                    break;
                case AssetType::Texture2D:
                    ImportTexture2D(path, {});
                    break;
                case AssetType::Clip:
                    ImportClip(path);
                    break;
                default:
                    break;
            }
        }
    }

    void AssetManager::LoadAll(Resources &resources) {
        for (auto [uuid, path]: m_AssetPaths) {
            switch (auto [assetUuid, type] = GetAssetMetaData(path + ".meta"); type) {
                case AssetType::Model:
                    LoadModel({assetUuid}, resources);
                    break;
                case AssetType::Texture2D:
                    LoadTexture2D({assetUuid}, resources);
                    break;
                case AssetType::Clip:
                    LoadClip({assetUuid}, resources);
                    break;
                default:
                    break;
            }
        }
    }

    ModelAssetHandle AssetManager::ImportModel(const std::string &filepath) {
        GenerateMetaFile(filepath, AssetType::Model);
        auto [uuid, type] = GetAssetMetaData(filepath + ".meta");
        m_AssetPaths[uuid.Hash()] = filepath;

        return {.uuid = uuid};
    }

    Texture2DAssetHandle AssetManager::ImportTexture2D(const std::string &filepath,
                                                       Graphics::Gl::Texture2D::Config config) {
        // TODO: Save config to metafile
        GenerateMetaFile(filepath, AssetType::Texture2D);
        auto [uuid, type] = GetAssetMetaData(filepath + ".meta");
        m_AssetPaths[uuid.Hash()] = filepath;

        return {.uuid = uuid};
    }

    ClipAssetHandle AssetManager::ImportClip(const std::string &filepath) {
        GenerateMetaFile(filepath, AssetType::Clip);
        auto [uuid, type] = GetAssetMetaData(filepath + ".meta");
        m_AssetPaths[uuid.Hash()] = filepath;

        return {.uuid = uuid};
    }

    Graphics::Model AssetManager::LoadModel(const ModelAssetHandle handle, Resources &resources) {
        const std::string filepath = m_AssetPaths[handle.uuid.Hash()];

        std::vector<MeshData> meshes = LoadModelMeshes(filepath);
        std::vector<MaterialData> materials = LoadModelMaterials(filepath);

        std::vector<Graphics::MaterialHandle> outputMaterials;

        outputMaterials.reserve(materials.size());
        for (auto &[diffusePath, specularPath, diffuseColor, specularColor]: materials) {
            Graphics::Material material;
            material.SetShader(resources.shaderStore.GetStandardShader());

            if (diffusePath) {
                const auto diff = LoadTexture2D(ImportTexture2D(diffusePath.value(), {}), resources);
                material.SetDiffuseTexture(diff);
            }

            if (specularPath) {
                const auto spec = LoadTexture2D(ImportTexture2D(specularPath.value(), {}), resources);
                material.SetSpecularTexture(spec);
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

        if (const auto [uuid, type] = GetAssetMetaData(filepath + ".meta"); type != AssetType::Model) {
            throw AssetError("Failed to import model: Mismatched asset types");
        }

        m_Models[handle.uuid.Hash()] = model;
        return model;
    }

    Graphics::Texture2DHandle AssetManager::LoadTexture2D(const Texture2DAssetHandle handle, Resources &resources) {
        const std::string filepath = m_AssetPaths[handle.uuid.Hash()];

        const auto image = LoadImage(filepath);
        const auto texture = resources.textureStore.Create(image, {});

        if (const auto [uuid, type] = GetAssetMetaData(filepath + ".meta"); type != AssetType::Texture2D) {
            throw AssetError("Failed to import texture: Mismatched asset types");
        }

        m_Textures[handle.uuid.Hash()] = texture;
        return texture;
    }

    Audio::ClipHandle AssetManager::LoadClip(const ClipAssetHandle handle, Resources &resources) {
        const std::string filepath = m_AssetPaths[handle.uuid.Hash()];

        auto &&audio = LoadAudioFile(filepath);
        const Audio::ClipHandle clip = resources.clipStore.Create(std::move(audio));

        if (const auto [uuid, type] = GetAssetMetaData(filepath + ".meta"); type != AssetType::Clip) {
            throw AssetError("Failed to import clip: Mismatched asset types");
        }

        m_Clips[handle.uuid.Hash()] = clip;
        return clip;
    }
    Graphics::Model AssetManager::GetModel(const ModelAssetHandle handle) {
        if (!m_Models.contains(handle.uuid.Hash())) {
            throw AssetError("Invalid UUID used to retrieve model: " + handle.uuid.ToString());
        }

        return m_Models[handle.uuid.Hash()];
    }

    Graphics::Texture2DHandle AssetManager::GetTexture2D(const Texture2DAssetHandle handle) {
        if (!m_Textures.contains(handle.uuid.Hash())) {
            throw AssetError("Invalid UUID used to retrieve model: " + handle.uuid.ToString());
        }

        return m_Textures[handle.uuid.Hash()];
    }

    Audio::ClipHandle AssetManager::GetClip(ClipAssetHandle handle) {
        if (!m_Clips.contains(handle.uuid.Hash())) {
            throw AssetError("Invalid UUID used to retrieve model: " + handle.uuid.ToString());
        }

        return m_Clips[handle.uuid.Hash()];
    }

    bool AssetManager::IsModelFile(const std::string &filepath) {
        const std::string extension = filepath.substr(filepath.find_last_of('.') + 1);

        return extension == "obj" || extension == "fbx" || extension == "gltf";
    }

    bool AssetManager::IsImageFile(const std::string &filepath) {
        const std::string extension = filepath.substr(filepath.find_last_of('.') + 1);

        return extension == "png" || extension == "jpg" || extension == "bmp";
    }

    bool AssetManager::IsAudioFile(const std::string &filepath) {
        const std::string extension = filepath.substr(filepath.find_last_of('.') + 1);

        return extension == "wav" || extension == ".ogg" || extension == "mp3";
    }

    void AssetManager::GenerateMetaFile(const std::string &filepath, const AssetType assetType) {
        const Uuid::Uuid uuid = Uuid::Uuid::FromName(filepath, Uuid::Uuid::UrlNamespace());

        Json::Json json;
        json.Add("uuid", uuid.ToString());
        json.Add("type", ToString(assetType));

        WriteFile(filepath + ".meta", json.ToString());
    }

    bool AssetManager::GenerateMetaFile(const std::string &filepath) {
        if (IsModelFile(filepath)) {
            GenerateMetaFile(filepath, AssetType::Model);
            return true;
        }

        if (IsImageFile(filepath)) {
            GenerateMetaFile(filepath, AssetType::Texture2D);
            return true;
        }

        if (IsAudioFile(filepath)) {
            GenerateMetaFile(filepath, AssetType::Clip);
            return true;
        }

        return false;
    }

    AssetMetaData AssetManager::GetAssetMetaData(const std::string &metafile) {
        const Json::Json json = Json::Json::Parse(ReadFile(metafile));

        const Uuid::Uuid uuid = Uuid::Uuid::FromString(json.Get<std::string>("uuid")).value();
        const AssetType assetType = FromString(json.Get<std::string>("type"));

        return {.uuid = uuid, .type = assetType};
    }

    void AssetManager::RegisterAssetPaths() {
        for (auto path: GetFilesInDirectory(m_AssetsDirectory, ".meta", true)) {
            auto [uuid, type] = GetAssetMetaData(path);
            path.erase(path.find_last_of('.'));
            m_AssetPaths[uuid.Hash()] = path;
        }
    }
} // namespace Pixf::Core::Files::Assets
