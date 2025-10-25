#include "ResourceManager.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "Gl/Shader.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

namespace Pixf::Core::Graphics {
    MeshHandle ResourceManager::CreateMesh(const std::vector<Vertex> &vertices, std::vector<unsigned int> indices) {
        if (indices.empty()) {
            // Fill indices with default data
            for (size_t i = 0; i < vertices.size(); i++) {
                indices.push_back(i);
            }
        }

        m_MeshCounter++;
        m_Meshes[m_MeshCounter] = std::make_shared<Mesh>(Mesh(vertices, indices));
        return MeshHandle(m_MeshCounter);
    }

    ShaderHandle ResourceManager::CreateShader(const std::string &vertSrc, const std::string &fragSrc) {
        m_ShaderCounter++;
        m_Shaders[m_ShaderCounter] = std::make_shared<Gl::Shader>(Gl::Shader(vertSrc, fragSrc));
        return ShaderHandle(m_ShaderCounter);
    }

    Error::Result<Texture2DHandle, ResourceError> ResourceManager::ImportTexture2D(const std::string &path,
                                                                                   const Gl::TextureConfig config) {
        if (m_Texture2DPaths.contains(path)) {
            if (m_Textures2D.at(m_Texture2DPaths[path])->GetConfig() == config) {
                return Texture2DHandle(m_Texture2DPaths[path]);
            }
        }

        m_Texture2DCounter++;
        auto tex = Gl::Texture2D::Load(path, config);
        if (tex.IsError()) {
            return ResourceError::FailedToLoad;
        }

        m_Textures2D[m_Texture2DCounter] = std::make_shared<Gl::Texture2D>(std::move(tex).Unwrap());
        m_Texture2DPaths[path] = m_Texture2DCounter;
        return Texture2DHandle(m_Texture2DCounter);
    }

    MaterialHandle ResourceManager::CreateMaterial() {
        Material material(*this);

        m_MaterialCounter++;
        m_Materials[m_MaterialCounter] = std::make_shared<Material>(material);
        return MaterialHandle(m_MaterialCounter);
    }

    MaterialHandle ResourceManager::CreateMaterial(const ShaderHandle shader) {
        Material material(*this, shader);

        m_MaterialCounter++;
        m_Materials[m_MaterialCounter] = std::make_shared<Material>(material);
        return MaterialHandle(m_MaterialCounter);
    }
    ModelHandle ResourceManager::CreateModel(const std::vector<MeshHandle> &meshes,
                                             const std::vector<MaterialHandle> &materials) {
        m_ModelCounter++;
        m_Models[m_ModelCounter] = std::make_shared<Model>(Model(meshes, materials));
        return ModelHandle(m_ModelCounter);
    }

    Error::Result<ModelHandle, ResourceError> ResourceManager::ImportModel(const std::string &path) {
        const auto result = Model::LoadModel(path, *this);
        if (result.IsError()) {
            return ResourceError::FailedToLoad;
        }

        const Model &model = result.Unwrap();

        m_ModelCounter++;
        m_Models[m_ModelCounter] = std::make_shared<Model>(model);
        return ModelHandle(m_ModelCounter);
    }

    Error::Result<std::shared_ptr<Mesh>, ResourceError> ResourceManager::GetMesh(const MeshHandle handle) {
        if (m_Meshes.find(handle.id) == m_Meshes.end()) {
            return ResourceError::NotFound;
        }

        return m_Meshes.at(handle.id);
    }

    Error::Result<std::shared_ptr<Gl::Shader>, ResourceError> ResourceManager::GetShader(const ShaderHandle handle) {
        if (m_Shaders.find(handle.id) == m_Shaders.end()) {
            return ResourceError::NotFound;
        }

        return m_Shaders.at(handle.id);
    }

    Error::Result<std::shared_ptr<Gl::Texture2D>, ResourceError>
    ResourceManager::GetTexture2D(const Texture2DHandle handle) {
        if (m_Textures2D.find(handle.id) == m_Textures2D.end()) {
            return ResourceError::NotFound;
        }

        return m_Textures2D.at(handle.id);
    }

    Error::Result<std::shared_ptr<Material>, ResourceError> ResourceManager::GetMaterial(const MaterialHandle handle) {
        if (m_Materials.find(handle.id) == m_Materials.end()) {
            return ResourceError::NotFound;
        }

        return m_Materials.at(handle.id);
    }

    Error::Result<std::shared_ptr<Model>, ResourceError> ResourceManager::GetModel(const ModelHandle handle) {
        if (m_Models.find(handle.id) == m_Models.end()) {
            return ResourceError::NotFound;
        }

        return m_Models.at(handle.id);
    }

    void ResourceManager::DeleteMesh(const MeshHandle handle) { m_Meshes.erase(handle.id); }

    void ResourceManager::DeleteShader(const ShaderHandle handle) { m_Shaders.erase(handle.id); }

    void ResourceManager::DeleteTexture2D(const Texture2DHandle handle) { m_Textures2D.erase(handle.id); }

    void ResourceManager::DeleteMaterial(const MaterialHandle handle) { m_Materials.erase(handle.id); }

    void ResourceManager::DeleteModel(const ModelHandle handle) { m_Models.erase(handle.id); }
} // namespace Pixf::Core::Graphics
