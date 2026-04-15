#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

#include "Math/Color.hpp"
#include "Debug/Log.hpp"
#include "assimp/color4.h"
#include "assimp/material.h"
#include "assimp/material.inl"
#include "assimp/mesh.h"
#include "assimp/types.h"

namespace Flock::FileIo {
    static constexpr f32 s_ImportScale = 1.0F;

    std::vector<MeshData> ReadModelMeshes(const std::filesystem::path &filePath) {
        Assimp::Importer importer;

        u32 flags = aiProcess_Triangulate |
                    aiProcess_GenNormals |
                    aiProcess_CalcTangentSpace |
                    aiProcess_MakeLeftHanded;

        if (const std::string extension = filePath.string().substr(filePath.string().find_last_of('.') + 1);
            extension == "obj" || extension == "fbx") {
            flags |= aiProcess_FlipUVs;
        }

        const aiScene *scene = importer.ReadFile(filePath.string(), flags);

        if (scene == nullptr) {
            Debug::LogErr("Failed to load model meshes: {}", importer.GetErrorString());
            return {};
        }

        std::string directory = filePath.string();
        directory.erase(directory.find_last_of('/') + 1);

        std::vector<MeshData> meshes;

        for (i32 i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh *mesh = scene->mMeshes[i];

            std::vector<Graphics::Vertex> vertices;
            std::vector<u32>              indices;

            for (u32 v = 0; v < mesh->mNumVertices; v++) {
                Graphics::Vertex vertex{};
                vertex.position.x = mesh->mVertices[v].x * s_ImportScale;
                vertex.position.y = mesh->mVertices[v].y * s_ImportScale;
                vertex.position.z = mesh->mVertices[v].z * s_ImportScale;

                if (mesh->HasNormals()) {
                    vertex.normal.x = mesh->mNormals[v].x;
                    vertex.normal.y = mesh->mNormals[v].y;
                    vertex.normal.z = mesh->mNormals[v].z;
                }

                if (mesh->HasTangentsAndBitangents()) {
                    vertex.tangent.x = mesh->mTangents[v].x;
                    vertex.tangent.y = mesh->mTangents[v].y;
                    vertex.tangent.z = mesh->mTangents[v].z;

                    vertex.bitangent.x = mesh->mBitangents[v].x;
                    vertex.bitangent.y = mesh->mBitangents[v].y;
                    vertex.bitangent.z = mesh->mBitangents[v].z;
                }

                if (mesh->mTextureCoords[0]) {
                    vertex.texCoords.x = mesh->mTextureCoords[0][v].x;
                    vertex.texCoords.y = mesh->mTextureCoords[0][v].y;
                }

                vertices.push_back(vertex);
            }

            for (u32 f = 0; f < mesh->mNumFaces; f++) {
                const aiFace face = mesh->mFaces[f];
                for (u32 idx = 0; idx < face.mNumIndices; idx++) {
                    indices.push_back(face.mIndices[idx]);
                }
            }

            meshes.push_back({.data = Graphics::MeshData{vertices, indices}, .materialIndex = mesh->mMaterialIndex});
        }

        return meshes;
    }

    std::vector<Graphics::Material> ReadModelMaterials(const std::filesystem::path &filePath) {
        Assimp::Importer importer;
        const aiScene *  scene = importer.ReadFile(filePath.string().c_str(), 0);

        if (scene == nullptr) {
            Debug::LogErr("Failed to load model materials: {}", importer.GetErrorString());
            return {};
        }

        std::string directory = filePath.string();
        directory.erase(directory.find_last_of('/') + 1);

        std::vector<Graphics::Material> materials;

        for (i32 i = 0; i < scene->mNumMaterials; i++) {
            const aiMaterial * material = scene->mMaterials[i];
            Graphics::Material outputMaterial;
            aiString           path;

            if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS) {
                outputMaterial.colorMapPath = directory + path.C_Str();
            }

            if (material->GetTexture(aiTextureType_METALNESS, 0, &path) == AI_SUCCESS) {
                outputMaterial.metallicMapPath = directory + path.C_Str();
            }

            if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path) == AI_SUCCESS) {
                outputMaterial.roughnessMapPath = directory + path.C_Str();
            }

            aiColor4D aiColor(1.0F, 1.0F, 1.0F, 1.0F);
            material->Get(AI_MATKEY_BASE_COLOR, aiColor);
            const Color4u8 color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255, aiColor.a * 255);
            outputMaterial.color = color;

            material->Get(AI_MATKEY_METALLIC_FACTOR, outputMaterial.metallic);
            material->Get(AI_MATKEY_ROUGHNESS_FACTOR, outputMaterial.roughness);

            materials.push_back(outputMaterial);
        }

        return materials;
    }
}
