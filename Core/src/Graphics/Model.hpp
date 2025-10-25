#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include <assimp/scene.h>

#include "Error/Result.hpp"
#include "ResourceManager.hpp"

namespace Pixf::Core::Graphics {
    enum class ModelError {
        None = 0,
        FailedToLoad,
    };

    class Model {
    public:
        static Error::Result<Model, ModelError> LoadModel(const std::string &path, ResourceManager &resourceManager);
        Model() = default;
        explicit Model(const std::vector<MeshHandle> &meshes, const std::vector<MaterialHandle> &materials);

        Model(const Model &) = default;
        Model(Model &&) = default;
        Model &operator=(const Model &) = default;
        Model &operator=(Model &&) = default;

        ~Model() = default;

        const std::vector<MeshHandle> &GetMeshes() const;
        const std::vector<MaterialHandle> &GetMaterials() const;

    private:
        std::vector<MeshHandle> m_Meshes;
        std::vector<MaterialHandle> m_Materials;

        void ProcessNode(const aiNode *node, const aiScene *scene, ResourceManager &resourceManager);
    };

} // namespace Pixf::Core::Graphics

#endif // MODEL_HPP
