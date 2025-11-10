#include "Material.hpp"

#include <optional>
#include <unordered_map>

#include "Assets/AssetManager.hpp"
#include "Common.hpp"

namespace Pixf::Core::Graphics {
    Material::Material(Assets::AssetManager &resourceManager) : m_AssetManager(resourceManager) {
        m_Shader = m_AssetManager.CreateShader();
        SetupDefaults();
    }

    Material::Material(Assets::AssetManager &resourceManager, const Assets::AssetHandle &shader) :
        m_Shader(shader), m_AssetManager(resourceManager) {
        SetupDefaults();
    }

    MaterialPropertyError Material::SetShader(const Assets::AssetHandle &handle) {
        if (const auto shader = m_AssetManager.GetShader(m_Shader); shader.IsError()) {
            return MaterialPropertyError::InvalidShader;
        }

        m_Shader = handle;
        return MaterialPropertyError::None;
    }

    Assets::AssetHandle Material::GetShader() const { return m_Shader; }

    void Material::SetDiffuse(const vec4 color) { SetVec4("uDiffuse", color); }

    void Material::SetDiffuseTexture2D(const std::optional<Assets::AssetHandle> &texture) {
        SetInt("uHasDiffuseTex", texture.has_value());
        if (texture.has_value()) {
            SetTexture2D("uDiffuseTex", texture.value());
        }
    }

    void Material::SetSpecular(const vec4 color) { SetVec4("uSpecular", color); }

    void Material::SetSpecularTexture2D(const std::optional<Assets::AssetHandle> &texture) {
        SetInt("uHasSpecularTex", texture.has_value());
        if (texture.has_value()) {
            SetTexture2D("uSpecularTex", texture.value());
        }
    }

    void Material::SetSpecularStrength(const float value) { SetFloat("uSpecularStrength", value); }

    void Material::SetShininess(const float value) { SetFloat("uShininess", value); }

    vec4 Material::GetDiffuse() const { return GetVec4("uDiffuse").UnwrapOr(vec4(0.0F)); }

    std::optional<Assets::AssetHandle> Material::GetDiffuseTexture2D() const {
        if (GetInt("uHasDiffuseTex").UnwrapOr(false)) {
            return GetTexture2D("uDiffuseTex").Unwrap();
        }

        return std::nullopt;
    }

    float Material::GetSpecularStrength() const { return GetFloat("uSpecularStrength").UnwrapOr(0.0F); }

    float Material::GetShininess() const { return GetFloat("uShininess").UnwrapOr(0.0F); }

    MaterialPropertyError Material::SetFloat(const std::string &name, const float value) {
        m_Floats[name] = value;
        return MaterialPropertyError::None;
    }

    MaterialPropertyError Material::SetInt(const std::string &name, const int value) {
        m_Ints[name] = value;
        return MaterialPropertyError::None;
    }

    MaterialPropertyError Material::SetVec2(const std::string &name, const vec2 value) {
        m_Vecs2D[name] = value;
        return MaterialPropertyError::None;
    }

    MaterialPropertyError Material::SetVec3(const std::string &name, const vec3 value) {
        m_Vecs3D[name] = value;
        return MaterialPropertyError::None;
    }

    MaterialPropertyError Material::SetVec4(const std::string &name, const vec4 value) {
        m_Vecs4D[name] = value;
        return MaterialPropertyError::None;
    }

    MaterialPropertyError Material::SetMat4(const std::string &name, const mat4 &value) {
        m_Mats[name] = value;
        return MaterialPropertyError::None;
    }

    MaterialPropertyError Material::SetTexture2D(const std::string &name, const Assets::AssetHandle &handle) {
        m_Textures2D[name] = handle;

        return MaterialPropertyError::None;
    }

    Error::Result<float, MaterialPropertyError> Material::GetFloat(const std::string &name) const {
        if (!m_Floats.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Floats.at(name);
    }

    Error::Result<int, MaterialPropertyError> Material::GetInt(const std::string &name) const {
        if (!m_Ints.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Ints.at(name);
    }

    Error::Result<char, MaterialPropertyError> Material::GetChar(const std::string &name) const {
        if (!m_Chars.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Chars.at(name);
    }

    Error::Result<vec2, MaterialPropertyError> Material::GetVec2(const std::string &name) const {
        if (!m_Vecs2D.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Vecs2D.at(name);
    }

    Error::Result<vec3, MaterialPropertyError> Material::GetVec3(const std::string &name) const {
        if (!m_Vecs3D.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Vecs3D.at(name);
    }

    Error::Result<vec4, MaterialPropertyError> Material::GetVec4(const std::string &name) const {
        if (!m_Vecs4D.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Vecs4D.at(name);
    }

    Error::Result<mat4, MaterialPropertyError> Material::GetMat4(const std::string &name) const {
        if (!m_Mats.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Mats.at(name);
    }

    Error::Result<Assets::AssetHandle, MaterialPropertyError> Material::GetTexture2D(const std::string &name) const {
        if (!m_Textures2D.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Textures2D.at(name);
    }

    void Material::Bind() const {
        // TODO: Get rid of Unwraps

        const auto shader = m_AssetManager.GetShader(m_Shader).Unwrap();
        shader->Bind();

        for (const auto &[name, value]: m_Floats) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Ints) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Chars) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Vecs2D) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Vecs3D) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Vecs4D) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Mats) {
            shader->SetUniform(name, value);
        }

        auto textureMap = shader->GetTextureMap();
        for (const auto &[name, value]: m_Textures2D) {
            m_AssetManager.GetTexture2D(value).Unwrap()->Bind(textureMap[name]);
        }
    }

    void Material::Unbind() {
        Gl::Shader::Unbind();
        for (size_t i = 0; i < g_MaxTextureCount; i++) {
            Gl::Texture2D::Unbind(i);
        }
    }

    void Material::SetupDefaults() {
        SetDiffuse(vec4(1.0F));
        SetDiffuseTexture2D(std::nullopt);

        SetSpecular(vec4(1.0F));
        SetSpecularTexture2D(std::nullopt);

        SetSpecularStrength(1.0F);
        SetShininess(2.0F);
    }
} // namespace Pixf::Core::Graphics
