#include "Material.hpp"

namespace Pixf::Core::Graphics {
    void Material::SetShader(const ShaderHandle shader) { m_Shader = shader; }

    void Material::SetFloat(const std::string &name, const float value) {
        SetUniform(name, UniformType::Float, value, m_Floats);
    }

    void Material::SetInt(const std::string &name, const int value) {
        SetUniform(name, UniformType::Int, value, m_Ints);
    }

    void Material::SetChar(const std::string &name, const char value) {
        SetUniform(name, UniformType::Char, value, m_Chars);
    }

    void Material::SetVector2(const std::string &name, const Math::Vector2f value) {
        SetUniform(name, UniformType::Vector2, value, m_Vector2s);
    }

    void Material::SetVector3(const std::string &name, const Math::Vector3f value) {
        SetUniform(name, UniformType::Vector3, value, m_Vector3s);
    }

    void Material::SetVector4(const std::string &name, const Math::Vector4f &value) {
        SetUniform(name, UniformType::Vector4, value, m_Vector4s);
    }

    void Material::SetColor3(const std::string &name, const Math::Color3u8 value) {
        SetUniform(name, UniformType::Color3, value, m_Color3s);
    }

    void Material::SetColor4(const std::string &name, const Math::Color4u8 value) {
        SetUniform(name, UniformType::Color4, value, m_Color4s);
    }

    void Material::SetTexture2D(const std::string &name, const Texture2DHandle value) {
        SetUniform(name, UniformType::Texture2D, value, m_Textures);
    }

    ShaderHandle Material::GetShader() const { return m_Shader; }

    std::optional<float> Material::GetFloat(const std::string &name) const {
        return GetUniform(name, UniformType::Float, m_Floats);
    }

    std::optional<int> Material::GetInt(const std::string &name) const {
        return GetUniform(name, UniformType::Int, m_Ints);
    }

    std::optional<char> Material::GetChar(const std::string &name) const {
        return GetUniform(name, UniformType::Char, m_Chars);
    }

    std::optional<Math::Vector2f> Material::GetVector2(const std::string &name) const {
        return GetUniform(name, UniformType::Vector2, m_Vector2s);
    }

    std::optional<Math::Vector3f> Material::GetVector3(const std::string &name) const {
        return GetUniform(name, UniformType::Vector3, m_Vector3s);
    }

    std::optional<Math::Vector4f> Material::GetVector4(const std::string &name) const {
        return GetUniform(name, UniformType::Vector4, m_Vector4s);
    }

    std::optional<Math::Color3u8> Material::GetColor3(const std::string &name) const {
        return GetUniform(name, UniformType::Color3, m_Color3s);
    }

    std::optional<Math::Color4u8> Material::GetColor4(const std::string &name) const {
        return GetUniform(name, UniformType::Color4, m_Color4s);
    }

    std::optional<Texture2DHandle> Material::GetTexture2D(const std::string &name) const {
        return GetUniform(name, UniformType::Texture2D, m_Textures);
    }

    std::optional<size_t> Material::FindUniform(const std::string &name) const {
        if (const auto it = std::find(m_Names.begin(), m_Names.end(), name); it != m_Names.end()) {
            return std::distance(m_Names.begin(), it);
        }

        return std::nullopt;
    }
} // namespace Pixf::Core::Graphics
