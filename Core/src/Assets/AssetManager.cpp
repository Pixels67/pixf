#include "AssetManager.hpp"

#include <boost/uuid.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "Debug/Logger.hpp"
#include "File/File.hpp"
#include "Graphics/Gl/Gl.hpp"
#include "Graphics/Gl/Shader.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Model.hpp"

namespace Pixf::Core::Graphics {
    AssetManager::~AssetManager() {
        PIXF_LOG_INFO("Cleaning resources");

        int vao, vbo, ebo, program, texture;

        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
        glGetIntegerv(GL_CURRENT_PROGRAM, &program);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);

        PIXF_LOG_TRACE("VAO bound: ", vao);
        PIXF_LOG_TRACE("VBO bound: ", vbo);
        PIXF_LOG_TRACE("EBO bound: ", ebo);
        PIXF_LOG_TRACE("Shader bound: ", program);
        PIXF_LOG_TRACE("Texture bound: ", texture);

        Mesh::Unbind();
        for (auto &[id, model]: m_Models) {
            model->Cleanup(*this);
        }

        for (auto &[id, mesh]: m_Meshes) {
            mesh->Cleanup();
        }

        Material::Unbind();
    }

    Error::Result<AssetHandle, AssetError> AssetManager::ImportTexture2D(const std::string &path,
                                                                         const Gl::TextureConfig config) {
        if (m_Texture2DPaths.contains(path)) {
            if (m_Textures2D.at(m_Texture2DPaths[path])->GetConfig() == config) {
                return AssetHandle(m_Texture2DPaths[path], AssetType::Texture2D);
            }
        }

        auto tex = Gl::Texture2D::Load(path, config);
        if (tex.IsError()) {
            return AssetError::FailedToLoad;
        }

        boost::uuids::uuid uuid = {};
        if (const auto metaFile = File::ReadFile(path + ".meta"); metaFile.IsSuccess()) {
            uuid = boost::uuids::string_generator()(metaFile.Unwrap());
        } else {
            uuid = m_UuidGenerator(path);
            File::WriteFile(path + ".meta", to_string(uuid));
        }

        m_Textures2D[uuid] = std::make_shared<Gl::Texture2D>(std::move(tex).Unwrap());
        m_Texture2DPaths[path] = uuid;
        return AssetHandle(uuid, AssetType::Texture2D);
    }

    Error::Result<AssetHandle, AssetError> AssetManager::ImportModel(const std::string &path) {
        const auto result = Model::LoadModel(path, *this);
        if (result.IsError()) {
            return AssetError::FailedToLoad;
        }

        const Model &model = result.Unwrap();

        boost::uuids::uuid uuid = {};
        if (const auto metaFile = File::ReadFile(path + ".meta"); metaFile.IsSuccess()) {
            uuid = boost::uuids::string_generator()(metaFile.Unwrap());
        } else {
            uuid = m_UuidGenerator(path);
            File::WriteFile(path + ".meta", to_string(uuid));
        }

        m_Models[uuid] = std::make_shared<Model>(model);
        return AssetHandle(uuid, AssetType::Model);
    }

    AssetHandle AssetManager::CreateShader(const std::string &vertSrc, const std::string &fragSrc) {
        const boost::uuids::uuid uuid = m_RandomUuidGenerator();
        m_Shaders[uuid] = std::make_shared<Gl::Shader>(Gl::Shader(vertSrc, fragSrc));
        return AssetHandle(uuid, AssetType::Shader);
    }

    AssetHandle AssetManager::CreateMaterial() {
        Material material(*this);

        const boost::uuids::uuid uuid = m_RandomUuidGenerator();
        m_Materials[uuid] = std::make_shared<Material>(material);
        return AssetHandle(uuid, AssetType::Material);
    }

    AssetHandle AssetManager::CreateMaterial(const AssetHandle &shader) {
        Material material(*this, shader);

        const boost::uuids::uuid uuid = m_RandomUuidGenerator();
        m_Materials[uuid] = std::make_shared<Material>(material);
        return AssetHandle(uuid, AssetType::Material);
    }

    Error::Result<std::shared_ptr<Gl::Shader>, AssetError> AssetManager::GetShader(const AssetHandle &handle) {
        if (!m_Shaders.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_Shaders.at(handle.uuid);
    }

    Error::Result<std::shared_ptr<Gl::Texture2D>, AssetError> AssetManager::GetTexture2D(const AssetHandle &handle) {
        if (!m_Textures2D.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_Textures2D.at(handle.uuid);
    }

    Error::Result<std::shared_ptr<Material>, AssetError> AssetManager::GetMaterial(const AssetHandle &handle) {
        if (!m_Materials.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_Materials.at(handle.uuid);
    }

    Error::Result<std::shared_ptr<Model>, AssetError> AssetManager::GetModel(const AssetHandle &handle) {
        if (!m_Models.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_Models.at(handle.uuid);
    }

    void AssetManager::DeleteShader(const AssetHandle &handle) { m_Shaders.erase(handle.uuid); }

    void AssetManager::DeleteTexture2D(const AssetHandle &handle) { m_Textures2D.erase(handle.uuid); }

    void AssetManager::DeleteMaterial(const AssetHandle &handle) { m_Materials.erase(handle.uuid); }

    void AssetManager::DeleteModel(const AssetHandle &handle) { m_Models.erase(handle.uuid); }

    AssetHandle AssetManager::CreateMesh(const std::vector<Vertex> &vertices, std::vector<unsigned int> indices) {
        if (indices.empty()) {
            // Fill indices with default data
            for (size_t i = 0; i < vertices.size(); i++) {
                indices.push_back(i);
            }
        }

        const boost::uuids::uuid uuid = m_RandomUuidGenerator();
        m_Meshes[uuid] = std::make_shared<Mesh>(Mesh(vertices, indices));
        return AssetHandle(uuid, AssetType::Mesh);
    }

    Error::Result<std::shared_ptr<Mesh>, AssetError> AssetManager::GetMesh(const AssetHandle &handle) {
        if (!m_Meshes.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_Meshes.at(handle.uuid);
    }

    void AssetManager::DeleteMesh(const AssetHandle &handle) { m_Meshes.erase(handle.uuid); }
} // namespace Pixf::Core::Assets
