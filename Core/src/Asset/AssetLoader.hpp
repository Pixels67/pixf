#ifndef FLK_ASSETLOADER_HPP
#define FLK_ASSETLOADER_HPP

#include "AssetHandle.hpp"
#include "Common.hpp"
#include "TypeId.hpp"
#include "FileIo/Image.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/Texture2D.hpp"
#include "FileIo/Model.hpp"
#include "FileIo/Pipeline.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Model.hpp"

namespace Flock::Asset {
    template<typename T>
    struct Loader;

    enum class PipelineType : u8 {
        Pbr,
        Unlit
    };

    struct AssetSlot {
        RefPtr<void> data;
        TypeId       typeId;
    };

    /**
     * @class AssetLoader
     * @brief Loads, unloads, and stores all the in-game assets.
     */
    class FLK_API AssetLoader {
        std::vector<std::optional<AssetSlot> > m_Assets;
        HashMap<std::string, AssetId>          m_AssetIds;
        std::vector<AssetId>                   m_FreeAssetIds;

        HashMap<PipelineType, AssetHandle<Graphics::Pipeline> > m_DefaultPipelines;

    public:
        /**
         * @brief Loads an asset from disk into memory.
         * @tparam T The asset type.
         * @param filePath The file path.
         * @return A handle to the asset if successful; std::nullopt otherwise.
         */
        template<typename T>
        std::optional<AssetHandle<T> > Load(const std::filesystem::path &filePath) {
            std::string pathStr = filePath.string();

            if (m_AssetIds.contains(pathStr)) {
                AssetId id     = m_AssetIds.at(pathStr);
                TypeId  typeId = m_Assets.at(id).value().typeId;

                if (typeId != GetTypeId<T>()) {
                    return std::nullopt;
                }

                return AssetHandle<T>{.id = id, .typeId = typeId};
            }

            auto asset = Loader<T>::Load(*this, filePath, std::nullopt);
            if (!asset.has_value()) {
                return std::nullopt;
            }

            RefPtr<T> ptr = std::make_shared<T>(std::move(asset.value()));

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
            return AssetHandle<T>{.id = id, .typeId = typeId};
        }

        /**
         * @brief Loads an asset from disk into memory with a configuration.
         * @tparam T The asset type.
         * @tparam C The asset configuration type.
         * @param filePath The file path.
         * @param config The configuration for the asset.
         * @return A handle to the asset if successful; std::nullopt otherwise.
         */
        template<typename T, typename C>
        std::optional<AssetHandle<T> > Load(const std::filesystem::path &filePath, C config) {
            std::string pathStr = filePath.string();

            if (m_AssetIds.contains(pathStr)) {
                AssetId id     = m_AssetIds.at(pathStr);
                TypeId  typeId = m_Assets.at(id).value().typeId;

                if (typeId != GetTypeId<T>()) {
                    return std::nullopt;
                }

                return AssetHandle<T>{.id = id, .typeId = typeId};
            }

            auto asset = Loader<T>::Load(*this, filePath, config);
            if (!asset.has_value()) {
                return std::nullopt;
            }

            RefPtr<T> ptr = std::make_shared<T>(std::move(asset.value()));

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
            return AssetHandle<T>{.id = id, .typeId = typeId};
        }

        /**
         * @brief Retrieves a reference to an asset.
         * @tparam T The asset type.
         * @param handle The handle to the asset.
         * @return A reference to the asset if successful; std::nullopt otherwise.
         */
        template<typename T>
        OptionalRef<T> Get(AssetHandle<T> handle) {
            if (handle.id >= m_Assets.size() || !m_Assets.at(handle.id).has_value()) {
                return std::nullopt;
            }

            if (GetTypeId<T>() != m_Assets[handle.id].value().typeId) {
                return std::nullopt;
            }

            return *static_pointer_cast<T>(m_Assets[handle.id].value().data);
        }

        /**
         * @brief Unloads an asset from memory.
         * @tparam T The asset type.
         * @param handle A handle to the asset.
         * @return true if successful; false otherwise.
         */
        template<typename T>
        bool Unload(AssetHandle<T> handle) {
            if (handle.id >= m_Assets.size()) {
                return false;
            }

            if (!m_Assets.at(handle.id).has_value()) {
                Debug::LogWrn("Trying to unload an asset that is already unloaded!");
                return false;
            }

            if (GetTypeId<T>() != m_Assets[handle.id].value().typeId) {
                return false;
            }

            m_Assets[handle.id] = std::nullopt;

            for (auto &[path, id]: m_AssetIds) {
                if (id == handle.id) {
                    m_AssetIds.erase(path);
                    break;
                }
            }

            m_FreeAssetIds.push_back(handle.id);

            return true;
        }

        /**
         * @brief Sets a default pipeline.
         * @param type The pipeline type.
         * @param pipeline The pipeline to set.
         * @return true if successful; false otherwise.
         */
        bool SetDefaultPipeline(const PipelineType type, const AssetHandle<Graphics::Pipeline> pipeline) {
            if (pipeline.id >= m_Assets.size() || !m_Assets.at(pipeline.id).has_value()) {
                return false;
            }

            m_DefaultPipelines[type] = pipeline;
            return true;
        }

        /**
         * @brief Retrieves the default pipeline of a stated type.
         * @param type The pipeline type.
         * @return The default pipeline of that type if found; std::nullopt otherwise.
         */
        std::optional<AssetHandle<Graphics::Pipeline> > GetDefaultPipeline(const PipelineType type) {
            if (!m_DefaultPipelines.contains(type)) {
                return std::nullopt;
            }

            return m_DefaultPipelines.at(type);
        }
    };

    template<>
    struct Loader<Graphics::Pipeline> {
        static std::optional<Graphics::Pipeline> Load(
            AssetLoader &                loader,
            const std::filesystem::path &filePath,
            std::optional<bool>) {
            return FileIo::ReadPipeline(filePath);
        }
    };

    template<>
    struct Loader<Graphics::Texture2D> {
        static std::optional<Graphics::Texture2D> Load(
            AssetLoader &                                loader,
            const std::filesystem::path &                filePath,
            const std::optional<Graphics::TextureConfig> config
        ) {
            return Graphics::Texture2D::FromImage(FileIo::ReadImage(filePath), config.value_or(Graphics::TextureConfig{}));
        }
    };

    template<>
    struct Loader<Graphics::Model> {
        static std::optional<Graphics::Model> Load(
            AssetLoader &                                loader,
            const std::filesystem::path &                filePath,
            const std::optional<Graphics::TextureConfig> config
        ) {
            using namespace Graphics;

            std::vector<FileIo::MeshData>     meshes    = std::move(FileIo::LoadModelMeshes(filePath));
            std::vector<FileIo::MaterialData> materials = std::move(FileIo::LoadModelMaterials(filePath));

            std::vector<Material> outputMaterials;

            outputMaterials.reserve(materials.size());
            for (auto &[color, metallic, roughness, colorMapPath, metallicMapPath, roughnessMapPath]: materials) {
                Material material;
                material.pipeline = loader.GetDefaultPipeline(PipelineType::Pbr).value();

                if (colorMapPath) {
                    const auto colorMap = loader.Load<Texture2D>(colorMapPath.value(), config);
                    material.colorMap   = colorMap;
                }

                if (metallicMapPath) {
                    const auto metallicMap = loader.Load<Texture2D>(metallicMapPath.value(), config);
                    material.metallicMap   = metallicMap;
                }

                if (roughnessMapPath) {
                    const auto roughnessMap = loader.Load<Texture2D>(roughnessMapPath.value(), config);
                    material.roughnessMap   = roughnessMap;
                }

                material.color     = color;
                material.metallic  = metallic;
                material.roughness = roughness;

                outputMaterials.push_back(material);
            }

            Model model;
            for (auto &[data, materialIndex]: meshes) {
                model.meshes.push_back(std::move(Mesh::Create(data).value()));
                model.materials.push_back(outputMaterials[materialIndex]);
            }

            return model;
        }
    };
}

#endif //FLK_ASSETLOADER_HPP
