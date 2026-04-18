#ifndef FLK_ASSETLOADER_HPP
#define FLK_ASSETLOADER_HPP

#include <concepts>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "AssetHandle.hpp"
#include "Common.hpp"
#include "TypeId.hpp"
#include "Audio/AudioClip.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/TextureAtlas.hpp"
#include "Gui/Font.hpp"
#include "FileIo/Audio.hpp"
#include "FileIo/File.hpp"
#include "FileIo/Font.hpp"
#include "FileIo/Image.hpp"
#include "Graphics/Texture.hpp"
#include "FileIo/Model.hpp"
#include "FileIo/Pipeline.hpp"
#include "Graphics/CubeMap.hpp"
#include "Graphics/Mesh.hpp"

namespace Flock::Asset {
    template<typename T>
    using AssetPool  = std::vector<std::optional<T> >;
    using AssetPaths = std::unordered_map<std::string, AssetId>;

    class AssetLoader;

    template<typename T>
    struct FLK_API Loader {
        static std::optional<T> Load(AssetLoader &, const std::filesystem::path &);
    };

    /**
     * @class AssetLoader
     * @brief Loads, unloads, and stores all the in-game assets.
     */
    class FLK_API AssetLoader {
        std::unordered_map<TypeId, std::shared_ptr<void> >                m_AssetPools;
        std::unordered_map<TypeId, AssetPaths>                            m_AssetPaths;
        std::unordered_map<std::string, AssetHandle<Graphics::Pipeline> > m_Pipelines;

    public:
        template<typename T>
        [[nodiscard]] AssetPool<T> &Pool() {
            if (!m_AssetPools.contains(GetTypeId<T>())) {
                m_AssetPools[GetTypeId<T>()] = std::make_shared<AssetPool<T> >();
            }

            return *std::static_pointer_cast<AssetPool<T> >(m_AssetPools[GetTypeId<T>()]);
        }

        template<typename T>
        [[nodiscard]] AssetPaths &Paths() {
            return m_AssetPaths[GetTypeId<T>()];
        }

        template<typename T>
        AssetHandle<T> Load(const std::filesystem::path &filePath) {
            if (filePath.empty()) {
                return AssetHandle<T>{};
            }

            if constexpr (std::same_as<T, Graphics::Pipeline>) {
                if (!filePath.empty() && filePath.string()[0] == '@') {
                    return GetPipeline(filePath.string().substr(1));
                }
            }

            AssetPool<T> &pool  = Pool<T>();
            AssetPaths &  paths = Paths<T>();

            if (paths.contains(filePath.string())) {
                return AssetHandle<T>{.id = paths.at(filePath.string()), .resolved = true, .filePath = filePath.string()};
            }

            std::optional<T> maybeAsset = Loader<T>::Load(*this, filePath);
            if (!maybeAsset) {
                return AssetHandle<T>{};
            }

            pool.push_back(std::move(maybeAsset.value()));
            const AssetId id         = pool.size() - 1;
            paths[filePath.string()] = id;

            return AssetHandle<T>{.id = id, .resolved = true, .filePath = filePath.string()};
        }

        template<typename T>
        AssetHandle<T> Register(T &&value) {
            AssetPool<T> &pool = Pool<T>();
            pool.push_back(std::move(value));
            const AssetId id = pool.size() - 1;

            return AssetHandle<T>{.id = id, .resolved = true};
        }

        template<typename T>
        void Resolve(AssetHandle<T> &handle) {
            if (handle.filePath.empty() || handle.resolved) {
                return;
            }

            handle          = Load<T>(handle.filePath);
            handle.resolved = true;
        }

        template<typename T>
        bool Unload(AssetHandle<T> handle) {
            Resolve(handle);
            if (!Has(handle)) {
                return false;
            }

            AssetPool<T> &pool = Pool<T>();
            pool.at(handle.id) = std::nullopt;
            return true;
        }

        template<typename T>
        bool Has(AssetHandle<T> handle) {
            Resolve(handle);
            const AssetPool<T> &pool = Pool<T>();
            return handle.id < pool.size() && pool.at(handle.id);
        }

        template<typename T>
        T *Get(AssetHandle<T> handle) {
            Resolve(handle);
            if (!Has(handle)) {
                return nullptr;
            }

            AssetPool<T> &pool = Pool<T>();
            return &pool.at(handle.id).value();
        }

        template<typename T>
        T *Get(const std::filesystem::path &filePath) {
            AssetHandle<T> handle = Load<T>(filePath);
            if (!handle.IsValid()) {
                return nullptr;
            }

            return Get<T>(handle);
        }

        bool SetPipeline(const std::string &name, AssetHandle<Graphics::Pipeline> pipeline) {
            Resolve(pipeline);
            if (!pipeline.IsValid()) {
                return false;
            }

            m_Pipelines[name] = pipeline;
            return true;
        }

        [[nodiscard]] AssetHandle<Graphics::Pipeline> GetPipeline(const std::string &name) const {
            if (m_Pipelines.contains(name)) {
                return m_Pipelines.at(name);
            }

            return AssetHandle<Graphics::Pipeline>{};
        }
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
    struct Loader<Graphics::TextureAtlas> {
        static std::optional<Graphics::TextureAtlas> Load(AssetLoader &, const std::filesystem::path &filePath) {
            const auto maybeText = FileIo::ReadText(filePath.string() + ".atlas");
            if (!maybeText) {
                return std::nullopt;
            }

            std::string text = maybeText.value();
            if (!text.find(',')) {
                return std::nullopt;
            }

            text.replace(text.find('\n'), std::string::npos, "");

            u32 x = std::stoul(text.substr(0, text.find_first_of(',')));
            u32 y = std::stoul(text.substr(text.find_first_of(',') + 1));
            return Graphics::TextureAtlas::FromImage(FileIo::ReadImage(filePath), {x, y});
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
            const std::vector<Material>   materials = FileIo::ReadModelMaterials(filePath);

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
