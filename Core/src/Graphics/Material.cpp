#include "Material.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <optional>
#include <unordered_map>

#include "Assets/AssetManager.hpp"

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

    void Material::SetDiffuse(const Math::Color4u8 &color) { SetColor4u8("uDiffuse", color); }

    void Material::SetDiffuseTexture2D(const std::optional<Assets::AssetHandle> &texture) {
        SetInt("uHasDiffuseTex", texture.has_value());
        if (texture.has_value()) {
            SetTexture2D("uDiffuseTex", texture.value());
        }
    }

    void Material::SetSpecular(const Math::Color4u8 &color) { SetColor4u8("uSpecular", color); }

    void Material::SetSpecularTexture2D(const std::optional<Assets::AssetHandle> &texture) {
        SetInt("uHasSpecularTex", texture.has_value());
        if (texture.has_value()) {
            SetTexture2D("uSpecularTex", texture.value());
        }
    }

    void Material::SetSpecularStrength(const float value) { SetFloat("uSpecularStrength", value); }

    void Material::SetShininess(const float value) { SetFloat("uShininess", value); }

    Math::Vector4f Material::GetDiffuse() const { return GetVector4f("uDiffuse").UnwrapOr(Math::Vector4f(0.0F)); }

    std::optional<Assets::AssetHandle> Material::GetDiffuseTexture2D() const {
        if (GetInt("uHasDiffuseTex").UnwrapOr(false)) {
            return GetTexture2D("uDiffuseTex").Unwrap();
        }

        return std::nullopt;
    }

    float Material::GetSpecularStrength() const { return GetFloat("uSpecularStrength").UnwrapOr(0.0F); }

    float Material::GetShininess() const { return GetFloat("uShininess").UnwrapOr(0.0F); }

    void Material::SetFloat(const std::string &name, const float value) {
        m_Floats[name] = value;
    }

    void Material::SetInt(const std::string &name, const int value) {
        m_Ints[name] = value;
    }

    void Material::SetVector2f(const std::string &name, const Math::Vector2f &value) {
        m_Vecs2[name] = value;
    }

    void Material::SetVector3f(const std::string &name, const Math::Vector3f& value) {
        m_Vecs3[name] = value;
    }

    void Material::SetVector4f(const std::string &name, const Math::Vector4f& value) {
        m_Vecs4[name] = value;
    }

    void Material::SetColor3u8(const std::string &name, const Math::Color3u8 &value) {
        m_Colors3[name] = value;
    }

    void Material::SetColor4u8(const std::string &name, const Math::Color4u8 &value) {
        m_Colors4[name] = value;
    }

    void Material::SetMatrix4f(const std::string &name, const Math::Matrix4f &value) {
        m_Mats[name] = value;
    }

    void Material::SetTexture2D(const std::string &name, const Assets::AssetHandle &handle) {
        m_Textures2D[name] = handle;
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

    Error::Result<Math::Vector2f, MaterialPropertyError> Material::GetVector2f(const std::string &name) const {
        if (!m_Vecs2.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Vecs2.at(name);
    }

    Error::Result<Math::Vector3f, MaterialPropertyError> Material::GetVector3f(const std::string &name) const {
        if (!m_Vecs3.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Vecs3.at(name);
    }

    Error::Result<Math::Vector4f, MaterialPropertyError> Material::GetVector4f(const std::string &name) const {
        if (!m_Vecs4.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Vecs4.at(name);
    }

    Error::Result<Math::Color3u8, MaterialPropertyError> Material::GetColor3u8(const std::string &name) const {
        if (!m_Colors3.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Colors3.at(name);
    }

    Error::Result<Math::Color4u8, MaterialPropertyError> Material::GetColor4u8(const std::string &name) const {
        if (!m_Colors4.contains(name)) {
            return MaterialPropertyError::NotFound;
        }

        return m_Colors4.at(name);
    }

    Error::Result<Math::Matrix4f, MaterialPropertyError> Material::GetMatrix4f(const std::string &name) const {
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
        const auto shader =
                m_AssetManager.GetShader(m_Shader).Unwrap("Failed to bind material: Invalid Material.shader");
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

        for (const auto &[name, value]: m_Vecs2) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Vecs3) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Vecs4) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Colors3) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Colors4) {
            shader->SetUniform(name, value);
        }

        for (const auto &[name, value]: m_Mats) {
            shader->SetUniform(name, value);
        }

        auto textureMap = shader->GetTextureMap();
        for (const auto &[name, value]: m_Textures2D) {
            m_AssetManager.GetTexture2D(value)
                    .Unwrap("Failed to bind material: " + name + " Texture2D with ID " +
                            uuids::to_string(value.GetUuid()) + " not found!")
                    ->Bind(textureMap[name]);
        }
    }

    void Material::Unbind() {
        Gl::Shader::Unbind();
        for (size_t i = 0; i < g_MaxTextureCount; i++) {
            Gl::Texture2D::Unbind(i);
        }
    }

    void Material::SetupDefaults() {
        SetDiffuse(Math::Color4u8(255));
        SetDiffuseTexture2D(std::nullopt);

        SetSpecular(Math::Color4u8(255));
        SetSpecularTexture2D(std::nullopt);

        SetSpecularStrength(1.0F);
        SetShininess(16.0F);
    }
} // namespace Pixf::Core::Graphics
