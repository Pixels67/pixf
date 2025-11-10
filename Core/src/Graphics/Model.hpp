#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include <assimp/scene.h>

#include "../Assets/AssetManager.hpp"
#include "Error/Result.hpp"

namespace Pixf::Core::Graphics {
    enum class ModelError {
        None = 0,
        FailedToLoad,
    };

    class Model {
    public:
        static Error::Result<Model, ModelError> LoadModel(const std::string &path, Assets::AssetManager &assetManager);
        Model() = default;
        explicit Model(const std::vector<Assets::AssetHandle> &meshes, const std::vector<Assets::AssetHandle> &materials);

        Model(const Model &) = default;
        Model(Model &&) = default;
        Model &operator=(const Model &) = default;
        Model &operator=(Model &&) = default;

        ~Model() = default;

        const std::vector<Assets::AssetHandle> &GetMeshes() const;
        const std::vector<Assets::AssetHandle> &GetMaterials() const;

        void Cleanup(Assets::AssetManager &assetManager) const;

    private:
        std::vector<Assets::AssetHandle> m_Meshes;
        std::vector<Assets::AssetHandle> m_Materials;

        void ProcessNode(const std::string &directory, const aiNode *node, const aiScene *scene, Assets::AssetManager &assetManager);
    };

} // namespace Pixf::Core::Graphics

#endif // MODEL_HPP
