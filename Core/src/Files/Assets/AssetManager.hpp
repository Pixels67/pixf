#ifndef PIXF_ASSETMANAGER_HPP
#define PIXF_ASSETMANAGER_HPP

#include "Common.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/Resources.hpp"
#include "Uuid/Uuid.hpp"

namespace Pixf::Core::Files::Assets {
    class PIXF_API AssetManager {
    public:
        explicit AssetManager(const std::string &assetsDirectory);

        Graphics::Model ImportModel(const Uuid::Uuid &uuid, Graphics::Resources &resources);
        Graphics::Texture2DHandle
        ImportTexture2D(const Uuid::Uuid &uuid, Graphics::Gl::Texture2D::Config config, Graphics::Resources &resources);

        Graphics::Model GetModel(const Uuid::Uuid &uuid);
        Graphics::Texture2DHandle GetTexture2D(const Uuid::Uuid &uuid);

    private:
        std::string m_AssetsDirectory;
        std::unordered_map<uint64_t, std::string> m_AssetPaths;

        std::unordered_map<uint64_t, Graphics::Model> m_Models;
        std::unordered_map<uint64_t, Graphics::Texture2DHandle> m_Textures;

        static Uuid::Uuid PathToUuid(const std::string &path);

        std::optional<std::string> UuidToPath(const Uuid::Uuid &uuid) const;
        void RegisterAssetPaths();
    };
} // namespace Pixf::Core::Files::Assets

#endif // PIXF_ASSETMANAGER_HPP
