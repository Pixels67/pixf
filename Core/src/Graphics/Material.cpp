#include "Material.hpp"

namespace Pixf::Core::Graphics {
    Material::Material() {
        SetDiffuse(Math::Color3u8::White());
        SetSpecular(Math::Color3u8::White());
        SetSpecularStrength(0.5F);
        SetShininess(8.0F);
    }

    void Material::SetDiffuse(const Math::Color3u8 &value) {
        SetColor3("uDiffuse", value);
    }

    void Material::SetSpecular(const Math::Color3u8 &value) {
        SetColor3("uSpecular", value);
    }

    void Material::SetDiffuseTexture(const Texture2DHandle value) {
        SetTexture2D("uDiffuseTex", value);
    }

    void Material::SetSpecularTexture(const Texture2DHandle value) {
        SetTexture2D("uSpecularTex", value);
    }

    void Material::SetSpecularStrength(const float value) {
        SetFloat("uSpecularStrength", value);
    }

    void Material::SetShininess(const float value) {
        SetFloat("uShininess", value);
    }

    std::optional<Math::Color3u8> Material::GetDiffuse() const {
        return GetColor3("uDiffuse");
    }

    std::optional<Math::Color3u8> Material::GetSpecular() const {
        return GetColor3("uSpecular");
    }

    std::optional<Texture2DHandle> Material::GetDiffuseTexture() const {
        return GetTexture2D("uDiffuseTex");
    }

    std::optional<Texture2DHandle> Material::GetSpecularTexture() const {
        return GetTexture2D("uSpecularTex");
    }

    std::optional<float> Material::GetSpecularStrength() const {
        return GetFloat("uSpecularStrength");
    }

    std::optional<float> Material::GetShininess() const {
        return GetFloat("uShininess");
    }

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

    void Material::SetVector2(const std::string &name, const Math::Vector2f &value) {
        SetUniform(name, UniformType::Vector2, value, m_Vector2s);
    }

    void Material::SetVector3(const std::string &name, const Math::Vector3f &value) {
        SetUniform(name, UniformType::Vector3, value, m_Vector3s);
    }

    void Material::SetVector4(const std::string &name, const Math::Vector4f &value) {
        SetUniform(name, UniformType::Vector4, value, m_Vector4s);
    }

    void Material::SetColor3(const std::string &name, const Math::Color3u8 &value) {
        SetUniform(name, UniformType::Color3, value, m_Color3s);
    }

    void Material::SetColor4(const std::string &name, const Math::Color4u8 &value) {
        SetUniform(name, UniformType::Color4, value, m_Color4s);
    }

    void Material::SetTexture2D(const std::string &name, const Texture2DHandle &value) {
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
    
    std::unordered_map<std::string, float> Material::GetFloatUniforms() const {
        std::unordered_map<std::string, float> uniforms;
        for (size_t i = 0; i < m_Floats.size(); i++) {
            if (!m_Floats[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Floats[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, int> Material::GetIntUniforms() const {
        std::unordered_map<std::string, int> uniforms;
        for (size_t i = 0; i < m_Ints.size(); i++) {
            if (!m_Ints[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Ints[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, char> Material::GetCharUniforms() const {
        std::unordered_map<std::string, char> uniforms;
        for (size_t i = 0; i < m_Chars.size(); i++) {
            if (!m_Chars[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Chars[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, Math::Vector2f> Material::GetVector2Uniforms() const {
        std::unordered_map<std::string, Math::Vector2f> uniforms;
        for (size_t i = 0; i < m_Vector2s.size(); i++) {
            if (!m_Vector2s[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Vector2s[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, Math::Vector3f> Material::GetVector3Uniforms() const {
        std::unordered_map<std::string, Math::Vector3f> uniforms;
        for (size_t i = 0; i < m_Vector3s.size(); i++) {
            if (!m_Vector3s[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Vector3s[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, Math::Vector4f> Material::GetVector4Uniforms() const {
        std::unordered_map<std::string, Math::Vector4f> uniforms;
        for (size_t i = 0; i < m_Vector4s.size(); i++) {
            if (!m_Vector4s[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Vector4s[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, Math::Color3u8> Material::GetColor3Uniforms() const {
        std::unordered_map<std::string, Math::Color3u8> uniforms;
        for (size_t i = 0; i < m_Color3s.size(); i++) {
            if (!m_Color3s[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Color3s[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, Math::Color4u8> Material::GetColor4Uniforms() const {
        std::unordered_map<std::string, Math::Color4u8> uniforms;
        for (size_t i = 0; i < m_Color4s.size(); i++) {
            if (!m_Color4s[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Color4s[i].value();
        }

        return uniforms;
    }
    
    std::unordered_map<std::string, Texture2DHandle> Material::GetTexture2DUniforms() const {
        std::unordered_map<std::string, Texture2DHandle> uniforms;
        for (size_t i = 0; i < m_Textures.size(); i++) {
            if (!m_Textures[i].has_value()) {
                continue;
            }

            uniforms[m_Names[i]] = m_Textures[i].value();
        }

        return uniforms;
    }

    void Material::Clear() {
        m_UniformCount = 0;
        m_Names.clear();
        m_Types.clear();
        m_Floats.clear();
        m_Ints.clear();
        m_Chars.clear();
        m_Vector2s.clear();
        m_Vector3s.clear();
        m_Vector4s.clear();
        m_Color3s.clear();
        m_Color4s.clear();
        m_Textures.clear();
    }

    std::optional<size_t> Material::FindUniform(const std::string &name) const {
        if (const auto it = std::find(m_Names.begin(), m_Names.end(), name); it != m_Names.end()) {
            return std::distance(m_Names.begin(), it);
        }

        return std::nullopt;
    }
} // namespace Pixf::Core::Graphics
