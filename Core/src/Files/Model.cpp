#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Debug/Logger.hpp"
#include "Image.hpp"

static constexpr float s_ImportScale = 1.0F;

namespace Pixf::Core::Files {
    Graphics::Model LoadModel(const std::string &path, Graphics::Resources &resources) {
        PIXF_CORE_LOG_INFO("Loading model: {}", path);

        Assimp::Importer importer;
        unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
                             aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded;

        if (const std::string extension = path.substr(path.find_last_of('.') + 1);
            extension == "obj" || extension == "fbx") {
            flags |= aiProcess_FlipUVs;
        }

        const aiScene *scene = importer.ReadFile(path, flags);

        if (scene == nullptr) {
            PIXF_CORE_LOG_ERROR("Failed to load model, Error: {}", importer.GetErrorString());
            return {};
        }

        std::string directory = path;
        directory.erase(directory.find_last_of('/') + 1);

        Graphics::Model model;
        const TextureCache textures = LoadTextures(scene, directory, resources);
        ProcessModelNode(model, directory, scene->mRootNode, scene, textures, resources);

        PIXF_CORE_LOG_INFO("Loaded model: {}", path);
        return model;
    }

    void ProcessModelNode(Graphics::Model &model,
                          const std::string &directory,
                          const aiNode *node,
                          const aiScene *scene,
                          const TextureCache &textures,
                          Graphics::Resources &resources) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

            Graphics::ModelElement element = {LoadMesh(mesh, resources), LoadMaterial(mat, textures, resources)};
            model.elements.push_back(element);

            PIXF_CORE_LOG_TRACE("Loaded mesh {}", node->mMeshes[i])
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessModelNode(model, directory, node->mChildren[i], scene, textures, resources);
        }
    }

    Graphics::MeshHandle LoadMesh(const aiMesh *mesh, Graphics::Resources &resources) {
        std::vector<Graphics::Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            Graphics::Vertex vertex{};
            vertex.position.x = mesh->mVertices[v].x * s_ImportScale;
            vertex.position.y = mesh->mVertices[v].y * s_ImportScale;
            vertex.position.z = mesh->mVertices[v].z * s_ImportScale;

            if (mesh->HasNormals()) {
                vertex.normal.x = mesh->mNormals[v].x;
                vertex.normal.y = mesh->mNormals[v].y;
                vertex.normal.z = mesh->mNormals[v].z;
            }

            if (mesh->mTextureCoords[0]) {
                vertex.texCoord.x = mesh->mTextureCoords[0][v].x;
                vertex.texCoord.y = mesh->mTextureCoords[0][v].y;
            }

            vertices.push_back(vertex);
        }

        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            const aiFace face = mesh->mFaces[f];
            for (unsigned int idx = 0; idx < face.mNumIndices; idx++) {
                indices.push_back(face.mIndices[idx]);
            }
        }

        return resources.meshStore.Create({vertices, indices});
    }

    TextureCache LoadTextures(const aiScene *scene, const std::string &directory, Graphics::Resources &resources) {
        TextureCache textures;
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            const aiMaterial *mat = scene->mMaterials[i];

            aiString diffPath;
            aiString specPath;

            if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath) == AI_SUCCESS &&
                !textures.contains(diffPath.C_Str())) {
                const Graphics::ImageData image = LoadImage(directory + diffPath.C_Str());
                textures[diffPath.C_Str()] = resources.textureStore.Create(image, {});
            }

            if (mat->GetTexture(aiTextureType_SPECULAR, 0, &specPath) == AI_SUCCESS &&
                !textures.contains(specPath.C_Str())) {
                const Graphics::ImageData image = LoadImage(directory + specPath.C_Str());
                textures[specPath.C_Str()] = resources.textureStore.Create(image, {});
            }
        }

        return textures;
    }

    Graphics::MaterialHandle
    LoadMaterial(const aiMaterial *material, const TextureCache &textures, Graphics::Resources &resources) {
        aiString diffPath;
        aiString specPath;

        std::optional<Graphics::Texture2DHandle> diffuse;
        std::optional<Graphics::Texture2DHandle> specular;

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath) == AI_SUCCESS) {
            if (textures.contains(diffPath.C_Str())) {
                diffuse = textures.at(diffPath.C_Str());
            } else {
                PIXF_CORE_LOG_ERROR("Failed to load material, texture not found: {}", diffPath.C_Str());
            }
        }

        if (material->GetTexture(aiTextureType_SPECULAR, 0, &specPath) == AI_SUCCESS) {
            if (textures.contains(specPath.C_Str())) {
                specular = textures.at(specPath.C_Str());
            } else {
                PIXF_CORE_LOG_ERROR("Failed to load material, texture not found: {}", specPath.C_Str());
            }
        }

        const Graphics::MaterialHandle matHandle = resources.materialStore.Create({});
        Graphics::Material &outputMaterial = resources.materialStore.Get(matHandle);
        outputMaterial.SetShader(resources.shaderStore.GetStandardShader());

        if (diffuse.has_value()) {
            outputMaterial.SetDiffuseTexture(diffuse.value());
        }

        aiColor3D aiDiffuseColor(1.0F, 1.0F, 1.0F);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuseColor);

        const Math::Color3u8 diffuseColor(aiDiffuseColor.r * 255, aiDiffuseColor.g * 255, aiDiffuseColor.b * 255);
        outputMaterial.SetDiffuse(diffuseColor);

        if (specular.has_value()) {
            outputMaterial.SetSpecularTexture(specular.value());
        }

        aiColor3D aiSpecularColor(1.0F, 1.0F, 1.0F);
        material->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecularColor);

        const Math::Color3u8 specularColor(aiSpecularColor.r * 255, aiSpecularColor.g * 255, aiSpecularColor.b * 255);
        outputMaterial.SetSpecular(specularColor);

        return matHandle;
    }
} // namespace Pixf::Core::Files
