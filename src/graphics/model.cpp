#include "model.h"

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#include "mesh.h"
#include "resource_manager.h"

namespace pixf::graphics {
Model::Model(const std::string& filepath, ResourceManager& resource_manager) {
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals |
                                      aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

  if (scene == nullptr) {
    std::cerr << importer.GetErrorString() << '\n';
    return;
  }

  ProcessNode(scene->mRootNode, scene, resource_manager);
}

Model::~Model() {}

void Model::Render(const MeshRenderConfig& render_config) const {
  for (unsigned int i = 0; i < meshes_.size(); i++) {
    render_config.resource_manager.GetMesh(meshes_[i])->Render(render_config);
  }
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene,
                        ResourceManager& resource_manager) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    const aiMesh* mesh = scene->mMeshes[i];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
      Vertex vertex;
      vertex.position.x = mesh->mVertices[v].x * 10.0F;
      vertex.position.y = mesh->mVertices[v].y * 10.0F;
      vertex.position.z = mesh->mVertices[v].z * 10.0F;

      if (mesh->HasNormals()) {
        vertex.normal.x = mesh->mNormals[v].x;
        vertex.normal.y = mesh->mNormals[v].y;
        vertex.normal.z = mesh->mNormals[v].z;
      }

      if (mesh->mTextureCoords[0]) {
        vertex.tex_coords.x = mesh->mTextureCoords[0][v].x;
        vertex.tex_coords.y = mesh->mTextureCoords[0][v].y;
      }

      vertices.push_back(vertex);
    }

    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
      const aiFace face = mesh->mFaces[f];
      for (unsigned int idx = 0; idx < face.mNumIndices; idx++) {
        indices.push_back(face.mIndices[idx]);
      }
    }

    meshes_.push_back(resource_manager.CreateMesh(vertices, indices));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene, resource_manager);
  }
}
}  // namespace pixf::graphics