#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Debug/Logger.hpp"
#include "Material.hpp"

constexpr float importScale = 1.0F;

struct aiScene;
namespace Pixf::Core::Graphics {
    Error::Result<Model, ModelError> Model::LoadModel(const std::string &path, ResourceManager &resourceManager) {
        PIXF_LOG_INFO("Importing model: ", path);

        Assimp::Importer importer;
        unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
                             aiProcess_CalcTangentSpace;

        if (const std::string extension = path.substr(path.find_last_of('.') + 1);
            extension == "obj" || extension == "fbx") {
            flags |= aiProcess_FlipUVs;
        }

        const aiScene *scene = importer.ReadFile(path, flags);

        if (scene == nullptr) {
            return ModelError::FailedToLoad;
        }

        Model model;
        model.ProcessNode(scene->mRootNode, scene, resourceManager);

        PIXF_LOG_INFO("Imported model: ", path);
        return Error::Result<Model, ModelError>(model);
    }

    Model::Model(const std::vector<MeshHandle> &meshes, const std::vector<MaterialHandle> &materials) :
        m_Meshes(meshes), m_Materials(materials) {}

    const std::vector<MeshHandle> &Model::GetMeshes() const { return m_Meshes; }

    const std::vector<MaterialHandle> &Model::GetMaterials() const { return m_Materials; }

    void Model::ProcessNode(const aiNode *node, const aiScene *scene, ResourceManager &resourceManager) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                Vertex vertex{};
                vertex.position.x = mesh->mVertices[v].x * importScale;
                vertex.position.y = mesh->mVertices[v].y * importScale;
                vertex.position.z = mesh->mVertices[v].z * importScale;

                if (mesh->HasNormals()) {
                    vertex.normal.x = mesh->mNormals[v].x;
                    vertex.normal.y = mesh->mNormals[v].y;
                    vertex.normal.z = mesh->mNormals[v].z;
                }

                if (mesh->mTextureCoords[0]) {
                    vertex.texCoords.x = mesh->mTextureCoords[0][v].x;
                    vertex.texCoords.y = mesh->mTextureCoords[0][v].y;
                }

                vertices.push_back(vertex);
            }

            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                const aiFace face = mesh->mFaces[f];
                for (unsigned int idx = 0; idx < face.mNumIndices; idx++) {
                    indices.push_back(face.mIndices[idx]);
                }
            }

            const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

            aiString diffPath;
            aiString specPath;

            // TODO: Get rid of Unwraps

            const MaterialHandle matHandle = resourceManager.CreateMaterial();
            Material &material = *resourceManager.GetMaterial(matHandle).Unwrap();

            if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath) == AI_SUCCESS) {
                material.SetDiffuseTexture2D(resourceManager.ImportTexture2D(diffPath.C_Str()).Unwrap());
            } else {
                aiColor3D diffuseColor(1.0F, 1.0F, 1.0F);
                mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
                material.SetDiffuse(vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0F));
            }

            if (mat->GetTexture(aiTextureType_SPECULAR, 0, &specPath) == AI_SUCCESS) {
                material.SetSpecularTexture2D(resourceManager.ImportTexture2D(specPath.C_Str()).Unwrap());
            } else {
                aiColor3D specularColor(1.0F, 1.0F, 1.0F);
                mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
                material.SetSpecular(vec4(specularColor.r, specularColor.g, specularColor.b, 1.0F));
            }

            m_Meshes.push_back(resourceManager.CreateMesh(vertices, indices));
            m_Materials.push_back(matHandle);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene, resourceManager);
        }
    }
} // namespace Pixf::Core::Graphics
