#include "AssetManager.hpp"

#include "Files/File.hpp"
#include "Files/Image.hpp"
#include "Files/Model.hpp"

namespace Pixf::Core::Files::Assets {
    AssetManager::AssetManager(const std::string &assetsDirectory) : m_AssetsDirectory(assetsDirectory) {
        RegisterAssetPaths();
    }

    Graphics::Model AssetManager::ImportModel(const Uuid::Uuid &uuid, Graphics::Resources &resources) {
        if (UuidToPath(uuid)) {
            auto model = LoadModel(UuidToPath(uuid).value(), resources);
            m_Models[uuid.Hash()] = model;
            return model;
        }

        throw FileError("Failed to load model: Path doesn't exist");
    }

    Graphics::Texture2DHandle AssetManager::ImportTexture2D(const Uuid::Uuid &uuid,
                                                            const Graphics::Gl::Texture2D::Config config,
                                                            Graphics::Resources &resources) {
        const auto image = LoadImage(UuidToPath(uuid).value());
        const auto texture = resources.textureStore.Create(image, config);

        m_Textures[uuid.Hash()] = texture;
        return texture;
    }

    Graphics::Model AssetManager::GetModel(const Uuid::Uuid &uuid) { return m_Models[uuid.Hash()]; }
    Graphics::Texture2DHandle AssetManager::GetTexture2D(const Uuid::Uuid &uuid) { return m_Textures[uuid.Hash()]; }

    Uuid::Uuid AssetManager::PathToUuid(const std::string &path) {
        if (FileExists(path + ".meta")) {
            const Uuid::Uuid uuid = Uuid::Uuid::FromString(ReadFile(path + ".meta")).value();
            return uuid;
        }

        const Uuid::Uuid uuid = Uuid::Uuid::FromName(path, Uuid::Uuid::UrlNamespace());
        WriteFile(path + ".meta", uuid.ToString());

        return uuid;
    }

    std::optional<std::string> AssetManager::UuidToPath(const Uuid::Uuid &uuid) const {
        if (!m_AssetPaths.contains(uuid.Hash())) {
            return std::nullopt;
        }

        return m_AssetPaths.at(uuid.Hash());
    }

    void AssetManager::RegisterAssetPaths() {
        for (auto path: GetFilesInDirectory(m_AssetsDirectory, ".meta", true)) {
            path.erase(path.find_last_of('.'));
            m_AssetPaths[PathToUuid(path).Hash()] = path;
        }
    }
} // namespace Pixf::Core::Files::Assets
