#ifndef PIXF_FILES_MODEL_HPP
#define PIXF_FILES_MODEL_HPP

#include <assimp/scene.h>

#include "Graphics/Model.hpp"
#include "Graphics/Resources.hpp"

namespace Pixf::Core::Files {
    using TextureCache = std::unordered_map<std::string, Graphics::Texture2DHandle>;

    PIXF_API Graphics::Model LoadModel(const std::string &path, Graphics::Resources &resources);

    void ProcessModelNode(Graphics::Model &model,
                          const std::string &directory,
                          const aiNode *node,
                          const aiScene *scene,
                          const TextureCache &textures,
                          Graphics::Resources &resources);

    Graphics::MeshHandle LoadMesh(const aiMesh *mesh, Graphics::Resources &resources);

    TextureCache LoadTextures(const aiScene *scene, const std::string &directory, Graphics::Resources &resources);

    Graphics::MaterialHandle
    LoadMaterial(const aiMaterial *material, const TextureCache &textures, Graphics::Resources &resources);
} // namespace Pixf::Core::Files

#endif // PIXF_FILES_MODEL_HPP
