#ifndef FLK_ASSETLOADER_HPP
#define FLK_ASSETLOADER_HPP

#include "Handle.hpp"
#include "Common.hpp"
#include "TypeId.hpp"
#include "Audio/AudioClip.hpp"
#include "FileIo/Audio.hpp"
#include "FileIo/Font.hpp"
#include "FileIo/Image.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/Texture.hpp"
#include "FileIo/Model.hpp"
#include "FileIo/Pipeline.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Model.hpp"
#include "Gui/Font.hpp"

namespace Flock::Asset {
    template<typename T>
    struct Loader;

    enum class PipelineType : u8 {
        Pbr,
        Unlit
    };

    struct AssetSlot {
        std::shared_ptr<void> data;
        TypeId                typeId;
    };

    /**
     * @class AssetLoader
     * @brief Loads, unloads, and stores all the in-game assets.
     */
    class FLK_API AssetLoader {
        std::vector<std::optional<AssetSlot> >   m_Assets;
        std::unordered_map<std::string, AssetId> m_AssetIds;
        std::vector<AssetId>                     m_FreeAssetIds;

        std::unordered_map<PipelineType, std::filesystem::path> m_DefaultPipelines;

    public:
        /**
         * @brief Loads an asset from disk into memory.
         * @tparam T The asset type.
         * @param filePath The file path.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool Load(const std::filesystem::path &filePath) {
            const std::string pathStr = filePath.string();

            if (m_AssetIds.contains(pathStr)) {
                const AssetId id     = m_AssetIds.at(pathStr);
                const TypeId  typeId = m_Assets.at(id).value().typeId;

                if (typeId != GetTypeId<T>()) {
                    return false;
                }

                return true;
            }

            auto asset = Loader<T>::Load(*this, filePath);
            if (!asset.has_value()) {
                return false;
            }

            std::shared_ptr<T> ptr = std::make_shared<T>(std::move(asset.value()));

            AssetId id     = 0;
            TypeId  typeId = 0;

            if (!m_FreeAssetIds.empty()) {
                id     = m_FreeAssetIds.back();
                typeId = GetTypeId<T>();

                m_FreeAssetIds.pop_back();

                m_Assets[id] = AssetSlot{.data = ptr, .typeId = typeId};
            } else {
                id     = m_Assets.size();
                typeId = GetTypeId<T>();

                m_Assets.emplace_back(AssetSlot{.data = ptr, .typeId = typeId});
            }

            m_AssetIds[pathStr] = id;
            return true;
        }

        /**
         * @brief Unloads an asset from memory.
         * @tparam T The asset type.
         * @param filePath The file path of the asset.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool Unload(std::filesystem::path filePath) {
            if (!IsLoaded<T>(filePath)) {
                return false;
            }

            const AssetId assetId = m_AssetIds[filePath.string()];

            m_Assets[assetId] = std::nullopt;
            m_AssetIds.erase(filePath.string());
            m_FreeAssetIds.push_back(assetId);

            return true;
        }

        template<typename T>
        bool IsLoaded(const std::filesystem::path &filePath) {
            const std::string str = filePath.string();
            if (!m_AssetIds.contains(str)) {
                return false;
            }

            if (GetTypeId<T>() != m_Assets[m_AssetIds[str]].value().typeId) {
                return false;
            }

            return true;
        }

        template<typename T>
        bool IsLoaded(Handle<T> handle) {
            if (handle.id >= m_Assets.size() || !m_Assets.at(handle.id).has_value()) {
                return false;
            }

            if (GetTypeId<T>() != m_Assets[handle.id].value().typeId) {
                return false;
            }

            return true;
        }

        /**
         * @brief Retrieves a reference to an asset.
         * @tparam T The asset type.
         * @param handle The handle to the asset.
         * @return A reference to the asset if successful; std::nullopt otherwise.
         */
        template<typename T>
        OptionalRef<T> Get(Handle<T> handle) {
            if (!IsLoaded(handle)) {
                return std::nullopt;
            }

            return *static_pointer_cast<T>(m_Assets[handle.id].value().data);
        }

        /**
         * @brief Retrieves a reference to an asset.
         * @tparam T The asset type.
         * @param filePath The file path of the asset.
         * @return A reference to the asset if successful; std::nullopt otherwise.
         */
        template<typename T>
        OptionalRef<T> Get(const std::filesystem::path filePath) {
            const std::string str = filePath.string();

            if constexpr (std::same_as<T, Graphics::Pipeline>) {
                if (str == "@PBR" && m_DefaultPipelines.contains(PipelineType::Pbr)) {
                    return Get<T>(m_DefaultPipelines[PipelineType::Pbr]);
                }
                if (str == "@Unlit" && m_DefaultPipelines.contains(PipelineType::Unlit)) {
                    return Get<T>(m_DefaultPipelines[PipelineType::Unlit]);
                }
                if (str[0] == '@') {
                    return std::nullopt;
                }
            }

            if (!IsLoaded<T>(filePath)) {
                if (Load<T>(filePath)) {
                    return Get<T>(filePath);
                }

                return std::nullopt;
            }

            return *static_pointer_cast<T>(m_Assets[m_AssetIds[filePath.string()]].value().data);
        }

        /**
         * @brief Unloads all assets of a specified type.
         * @tparam T The asset type.
         */
        template<typename T>
        void UnloadAll() {
            for (auto &asset: m_Assets) {
                if (!asset.has_value()) {
                    continue;
                }

                if (asset.value().typeId != GetTypeId<T>()) {
                    continue;
                }

                asset = std::nullopt;
            }
        }

        /**
         * @brief Sets a default pipeline.
         * @param type The pipeline type.
         * @param filePath The pipeline to set.
         * @return true if successful; false otherwise.
         */
        bool SetDefaultPipeline(const PipelineType type, const std::filesystem::path &filePath) {
            if (!Load<Graphics::Pipeline>(filePath)) {
                return false;
            }

            m_DefaultPipelines[type] = filePath.string();
            return true;
        }

        /**
         * @brief Unloads all assets.
         */
        void UnloadAll();
    };

    template<>
    struct Loader<Graphics::Pipeline> {
        static std::optional<Graphics::Pipeline> Load(AssetLoader &, const std::filesystem::path &filePath) {
            return FileIo::ReadPipeline(filePath);
        }
    };

    template<>
    struct Loader<Graphics::Texture> {
        static std::optional<Graphics::Texture> Load(AssetLoader &, const std::filesystem::path &filePath) {
            return Graphics::Texture::FromImage(FileIo::ReadImage(filePath));
        }
    };

    template<>
    struct Loader<Graphics::CubeMap> {
        static std::optional<Graphics::CubeMap> Load(AssetLoader &, const std::filesystem::path &filePath) {
            if (filePath.empty()) {
                return std::nullopt;
            }

            const std::string str  = filePath.string();
            const std::string ext  = str.substr(str.find_last_of('.'));
            const std::string name = str.substr(0, str.find_last_of('.'));

            return Graphics::CubeMap::FromImages(
                FileIo::ReadImage(name + "_right" + ext),
                FileIo::ReadImage(name + "_left" + ext),
                FileIo::ReadImage(name + "_up" + ext),
                FileIo::ReadImage(name + "_down" + ext),
                FileIo::ReadImage(name + "_forward" + ext),
                FileIo::ReadImage(name + "_back" + ext)
            );
        }
    };

    template<>
    struct Loader<Audio::AudioClip> {
        static std::optional<Audio::AudioClip> Load(AssetLoader &, const std::filesystem::path &filePath) {
            return FileIo::ReadAudioClip(filePath);
        }
    };

    template<>
    struct Loader<Graphics::Model> {
        static std::optional<Graphics::Model> Load(AssetLoader &loader, const std::filesystem::path &filePath) {
            using namespace Graphics;

            std::vector<FileIo::MeshData> meshes    = FileIo::ReadModelMeshes(filePath);
            std::vector<Material>         materials = FileIo::ReadModelMaterials(filePath);
            for (auto &[pipeline, color, metallic, roughness, colorMapPath, metallicMapPath, roughnessMapPath]:
                 materials) {
                if (pipeline != "@PBR" && pipeline != "@Unlit") {
                    loader.Load<Pipeline>(pipeline);
                }

                if (colorMapPath != "") {
                    loader.Load<Texture>(colorMapPath);
                }

                if (metallicMapPath != "") {
                    loader.Load<Texture>(metallicMapPath);
                }

                if (roughnessMapPath != "") {
                    loader.Load<Texture>(roughnessMapPath);
                }
            }

            Model model;
            for (auto &[data, materialIndex]: meshes) {
                model.objects.push_back({
                    .mesh     = Mesh::Create(data).value(),
                    .material = materials[materialIndex]
                });
            }

            return model;
        }
    };

    template<>
    struct Loader<Gui::Font> {
        static std::optional<Gui::Font> Load(AssetLoader &, const std::filesystem::path &filePath) {
            return FileIo::ReadFont(filePath);
        }
    };
}

#endif //FLK_ASSETLOADER_HPP
