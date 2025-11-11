#include "AssetManager.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/uuid.hpp>

#include "Audio/AudioEngine.hpp"
#include "Debug/Logger.hpp"
#include "File/File.hpp"
#include "Graphics/Gl/Gl.hpp"
#include "Graphics/Gl/Shader.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Model.hpp"
#include "Json/Json.hpp"

using namespace boost;

const std::string assetPath = "Assets/";

namespace Pixf::Core::Assets {
    using namespace Graphics;

    AssetManager::AssetManager() {
        for (auto path: File::GetFilesInDirectory(assetPath, ".meta", true)) {
            try {
                json::object json = json::parse(File::ReadFile(path).Unwrap()).as_object();
                uuids::uuid uuid = uuids::string_generator()(json["uuid"].as_string().c_str());
                path.erase(path.find_last_of('.'));
                m_AssetPaths[uuid] = path;
            } catch ([[maybe_unused]] const system::system_error &e) {
            }
        }
    }

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

        for (auto &[id, clip]: m_AudioClips) {
            clip->Cleanup();
        }
    }

    Error::Result<AssetHandle, AssetError> AssetManager::ImportTexture2D(const std::string &path,
                                                                         const Gl::TextureConfig config) {
        if (m_Texture2DPaths.contains(path)) {
            if (m_Textures2D.at(m_Texture2DPaths[path])->GetConfig() == config) {
                return AssetHandle(*this, m_Texture2DPaths[path], AssetType::Texture2D);
            }
        }

        auto tex = Gl::Texture2D::Load(path, config);
        if (tex.IsError()) {
            return AssetError::FailedToLoad;
        }

        const uuids::uuid uuid = GetUuid(path, AssetType::Texture2D).Unwrap();
        m_Textures2D[uuid] = std::make_shared<Gl::Texture2D>(std::move(tex).Unwrap());
        m_Texture2DPaths[path] = uuid;
        return AssetHandle(*this, uuid, AssetType::Texture2D);
    }

    Error::Result<AssetHandle, AssetError> AssetManager::ImportModel(const std::string &path) {
        if (m_ModelPaths.contains(path)) {
            return AssetHandle(*this, m_ModelPaths[path], AssetType::Model);
        }

        const auto result = Model::LoadModel(path, *this);
        if (result.IsError()) {
            return AssetError::FailedToLoad;
        }

        const Model &model = result.Unwrap();

        const uuids::uuid uuid = GetUuid(path, AssetType::Model).Unwrap();
        m_Models[uuid] = std::make_shared<Model>(model);
        m_ModelPaths[path] = uuid;
        return AssetHandle(*this, uuid, AssetType::Model);
    }

    Error::Result<AssetHandle, AssetError> AssetManager::ImportAudioClip(const std::string &path) {
        if (m_AudioClipPaths.contains(path)) {
            return AssetHandle(*this, m_AudioClipPaths[path], AssetType::AudioClip);
        }

        const uuids::uuid uuid = GetUuid(path, AssetType::AudioClip).Unwrap();
        m_AudioClips[uuid] = std::make_shared<Audio::AudioClip>();

        if (const auto result = m_AudioClips[uuid]->Load(path); result != Audio::AudioClipError::None) {
            File::DeleteFile(path + ".meta");
            return AssetError::FailedToLoad;
        }

        m_AudioClipPaths[path] = uuid;
        return AssetHandle(*this, uuid, AssetType::AudioClip);
    }

    AssetHandle AssetManager::CreateShader(const std::string &vertSrc, const std::string &fragSrc) {
        const uuids::uuid uuid = m_RandomUuidGenerator();
        m_Shaders[uuid] = std::make_shared<Gl::Shader>(Gl::Shader(vertSrc, fragSrc));
        return AssetHandle(*this, uuid, AssetType::Shader);
    }

    AssetHandle AssetManager::CreateMaterial() {
        Material material(*this);

        const uuids::uuid uuid = m_RandomUuidGenerator();
        m_Materials[uuid] = std::make_shared<Material>(material);
        return AssetHandle(*this, uuid, AssetType::Material);
    }

    AssetHandle AssetManager::CreateMaterial(const AssetHandle &shader) {
        Material material(*this, shader);

        const uuids::uuid uuid = m_RandomUuidGenerator();
        m_Materials[uuid] = std::make_shared<Material>(material);
        return AssetHandle(*this, uuid, AssetType::Material);
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

    Error::Result<std::shared_ptr<Audio::AudioClip>, AssetError> AssetManager::GetAudioClip(const AssetHandle &handle) {
        if (!m_AudioClips.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_AudioClips.at(handle.uuid);
    }

    void AssetManager::DeleteShader(const AssetHandle &handle) { m_Shaders.erase(handle.uuid); }

    void AssetManager::DeleteTexture2D(const AssetHandle &handle) { m_Textures2D.erase(handle.uuid); }

    void AssetManager::DeleteMaterial(const AssetHandle &handle) { m_Materials.erase(handle.uuid); }

    void AssetManager::DeleteModel(const AssetHandle &handle) { m_Models.erase(handle.uuid); }

    void AssetManager::DeleteAudioClip(const AssetHandle &handle) { m_AudioClips.erase(handle.uuid); }

    AssetHandle AssetManager::CreateMesh(const std::vector<Vertex> &vertices, std::vector<unsigned int> indices) {
        if (indices.empty()) {
            // Fill indices with default data
            for (size_t i = 0; i < vertices.size(); i++) {
                indices.push_back(i);
            }
        }

        const uuids::uuid uuid = m_RandomUuidGenerator();
        m_Meshes[uuid] = std::make_shared<Mesh>(Mesh(vertices, indices));
        return AssetHandle(*this, uuid, AssetType::Mesh);
    }

    Error::Result<std::shared_ptr<Mesh>, AssetError> AssetManager::GetMesh(const AssetHandle &handle) {
        if (!m_Meshes.contains(handle.uuid)) {
            return AssetError::NotFound;
        }

        return m_Meshes.at(handle.uuid);
    }

    void AssetManager::DeleteMesh(const AssetHandle &handle) { m_Meshes.erase(handle.uuid); }

    std::optional<std::string> AssetManager::GetAssetPath(const AssetHandle &handle) {
        return GetAssetPath(handle.uuid);
    }

    std::optional<std::string> AssetManager::GetAssetPath(const uuids::uuid &uuid) {
        if (m_AssetPaths.contains(uuid)) {
            return m_AssetPaths.at(uuid);
        }

        return std::nullopt;
    }

    Error::Result<uuids::uuid, AssetError> AssetManager::GetUuid(const std::string &path, const AssetType type) const {
        uuids::uuid uuid = {};
        if (const auto metaFile = File::ReadFile(path + ".meta"); metaFile.IsSuccess()) {
            json::object json;

            try {
                json = json::parse(metaFile.Unwrap()).as_object();
            } catch (const system::system_error &e) {
                PIXF_LOG_ERROR("Failed to parse JSON object. Error: ", e.what());
                return AssetError::FailedToParseMetaFile;
            }

            if (json["type"].as_string() != ToString(type)) {
                return AssetError::MismatchedType;
            }

            uuid = uuids::string_generator()(json["uuid"].as_string().c_str());
        } else {
            uuid = m_UuidGenerator(path);
            json::object json;
            json["uuid"] = to_string(uuid);
            json["type"] = ToString(type);

            File::WriteFile(path + ".meta", Json::ToPrettyJson(json));
        }

        return uuid;
    }
} // namespace Pixf::Core::Assets
