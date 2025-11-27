#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Debug/Logger.hpp"
#include "Image.hpp"

static constexpr float s_ImportScale = 1.0F;

namespace Pixf::Core::Files {
    std::vector<MeshData> LoadModelMeshes(const std::string &filepath) {
        Assimp::Importer importer;
        unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
                             aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded;

        if (const std::string extension = filepath.substr(filepath.find_last_of('.') + 1);
            extension == "obj" || extension == "fbx") {
            flags |= aiProcess_FlipUVs;
        }

        const aiScene *scene = importer.ReadFile(filepath, flags);

        if (scene == nullptr) {
            PIXF_CORE_LOG_ERROR("Failed to load model mesh, Error: {}", importer.GetErrorString());
            return {};
        }

        std::string directory = filepath;
        directory.erase(directory.find_last_of('/') + 1);

        std::vector<MeshData> meshes;

        for (int i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh *mesh = scene->mMeshes[i];

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

            meshes.push_back({.data = Graphics::MeshData{vertices, indices}, .materialIndex = mesh->mMaterialIndex});
        }

        return meshes;
    }

    std::vector<MaterialData> LoadModelMaterials(const std::string &filepath) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filepath, 0);

        if (scene == nullptr) {
            PIXF_CORE_LOG_ERROR("Failed to load model 2D texture, Error: {}", importer.GetErrorString());
            return {};
        }

        std::string directory = filepath;
        directory.erase(directory.find_last_of('/') + 1);

        std::vector<MaterialData> materials;

        for (int i = 0; i < scene->mNumMaterials; i++) {
            const aiMaterial *material = scene->mMaterials[i];
            MaterialData outputMaterial;
            aiString diffPath;
            aiString specPath;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath) == AI_SUCCESS) {
                outputMaterial.diffusePath = directory + diffPath.C_Str();
            } else {
                outputMaterial.diffusePath = std::nullopt;
            }

            if (material->GetTexture(aiTextureType_SPECULAR, 0, &specPath) == AI_SUCCESS) {
                outputMaterial.specularPath = directory + specPath.C_Str();
            } else {
                outputMaterial.specularPath = std::nullopt;
            }

            aiColor3D aiDiffuseColor(1.0F, 1.0F, 1.0F);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuseColor);
            const Math::Color3u8 diffuseColor(aiDiffuseColor.r * 255, aiDiffuseColor.g * 255, aiDiffuseColor.b * 255);
            outputMaterial.diffuseColor = diffuseColor;

            aiColor3D aiSpecularColor(1.0F, 1.0F, 1.0F);
            material->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecularColor);
            const Math::Color3u8 specularColor(aiSpecularColor.r * 255,
                                               aiSpecularColor.g * 255,
                                               aiSpecularColor.b * 255);
            outputMaterial.specularColor = specularColor;

            materials.push_back(outputMaterial);
        }

        return materials;
    }
} // namespace Pixf::Core::Files
