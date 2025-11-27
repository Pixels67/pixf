#ifndef PIXF_ASSETMANAGER_HPP
#define PIXF_ASSETMANAGER_HPP

#include "AssetHandle.hpp"
#include "Common.hpp"
#include "Error/Error.hpp"
#include "Graphics/Model.hpp"
#include "Resources.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Files::Assets {
    class AssetError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API AssetManager {
    public:
        explicit AssetManager(const std::string &assetsDirectory);

        ModelAssetHandle ImportModel(const std::string &filepath, Resources &resources);
        Texture2DAssetHandle
        ImportTexture2D(const std::string &filepath, Graphics::Gl::Texture2D::Config config, Resources &resources);
        ClipAssetHandle ImportClip(const std::string &filepath, Resources &resources);

        Graphics::Model GetModel(const ModelAssetHandle &model);
        Graphics::Texture2DHandle GetTexture2D(const Texture2DAssetHandle &texture);
        Audio::ClipHandle GetClip(const ClipAssetHandle &clip);

    private:
        std::string m_AssetsDirectory;
        std::unordered_map<uint64_t, std::string> m_AssetPaths;

        std::unordered_map<uint64_t, Graphics::Model> m_Models;
        std::unordered_map<uint64_t, Graphics::Texture2DHandle> m_Textures;
        std::unordered_map<uint64_t, Audio::ClipHandle> m_Clips;

        Uuid::Uuid PathToUuid(const std::string &filepath);
        std::optional<std::string> UuidToPath(const Uuid::Uuid &uuid) const;

        void GenerateMetaFile(const std::string &filepath);
        void RegisterAssetPaths();
    };
} // namespace Pixf::Core::Files::Assets

#endif // PIXF_ASSETMANAGER_HPP
