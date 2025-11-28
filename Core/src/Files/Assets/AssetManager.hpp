#ifndef PIXF_ASSETMANAGER_HPP
#define PIXF_ASSETMANAGER_HPP

#include "AssetHandle.hpp"
#include "Common.hpp"
#include "Error/Error.hpp"
#include "Graphics/Model.hpp"
#include "Resources.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Files::Assets {
    class PIXF_API AssetError final : public Error::Error {
    public:
        using Error::Error;
    };

    enum class AssetType : uint8_t {
        None = 0,
        Model,
        Texture2D,
        Clip,
    };

    std::string ToString(AssetType type);
    AssetType FromString(const std::string &str);

    struct AssetMetaData {
        Uuid::Uuid uuid;
        AssetType type;
    };

    class PIXF_API AssetManager {
    public:
        explicit AssetManager(const std::string &assetsDirectory);

        void ImportAll();
        void LoadAll(Resources &resources);

        ModelAssetHandle ImportModel(const std::string &filepath);
        Texture2DAssetHandle ImportTexture2D(const std::string &filepath, Graphics::Gl::Texture2D::Config config);
        ClipAssetHandle ImportClip(const std::string &filepath);

        Graphics::Model LoadModel(ModelAssetHandle handle, Resources &resources);
        Graphics::Texture2DHandle LoadTexture2D(Texture2DAssetHandle handle, Resources &resources);
        Audio::ClipHandle LoadClip(ClipAssetHandle handle, Resources &resources);

        Graphics::Model GetModel(ModelAssetHandle handle);
        Graphics::Texture2DHandle GetTexture2D(Texture2DAssetHandle handle);
        Audio::ClipHandle GetClip(ClipAssetHandle handle);

    private:
        std::string m_AssetsDirectory;
        std::unordered_map<uint64_t, std::string> m_AssetPaths;

        std::unordered_map<uint64_t, Graphics::Model> m_Models;
        std::unordered_map<uint64_t, Graphics::Texture2DHandle> m_Textures;
        std::unordered_map<uint64_t, Audio::ClipHandle> m_Clips;

        static bool IsModelFile(const std::string &filepath);
        static bool IsImageFile(const std::string &filepath);
        static bool IsAudioFile(const std::string &filepath);

        static void GenerateMetaFile(const std::string &filepath, AssetType assetType);
        static bool GenerateMetaFile(const std::string &filepath);
        static AssetMetaData GetAssetMetaData(const std::string &metafile);

        void RegisterAssetPaths();
    };
} // namespace Pixf::Core::Files::Assets

#endif // PIXF_ASSETMANAGER_HPP
