#include "model.h"

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#include "mesh.h"
#include "resource_manager.h"

constexpr float IMPORT_SCALE_FACTOR = 0.01F;

namespace pixf::graphics {
Model::Model(const std::string& filepath, ResourceManager& resource_manager) {
  Assimp::Importer importer;

  unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals |
                       aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

  if (const std::string extension = filepath.substr(filepath.find_last_of('.') + 1);
      extension == "obj" || extension == "fbx") {
    flags |= aiProcess_FlipUVs;
  }

  const aiScene* scene = importer.ReadFile(filepath, flags);

  if (scene == nullptr) {
    std::cerr << importer.GetErrorString() << '\n';
    return;
  }

  ProcessNode(scene->mRootNode, scene, resource_manager);
}

Model::~Model() {}

void Model::Render(const ModelRenderConfig& render_config,
                   const ResourceManager& resource_manager) const {
  for (unsigned int i = 0; i < meshes_.size(); i++) {
    Material material = {};
    if (i < render_config.materials.size()) {
      material = render_config.materials[i];
    }

    MeshRenderConfig config = {render_config.camera,    render_config.point_lights,
                               render_config.proj,      material,
                               render_config.transform, render_config.ambient_light};

    resource_manager.GetMesh(meshes_[i])->Render(config, resource_manager);
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
      vertex.position.x = mesh->mVertices[v].x * IMPORT_SCALE_FACTOR;
      vertex.position.y = mesh->mVertices[v].y * IMPORT_SCALE_FACTOR;
      vertex.position.z = mesh->mVertices[v].z * IMPORT_SCALE_FACTOR;

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