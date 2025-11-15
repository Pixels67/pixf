#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <unordered_map>
#include <optional>

#include "Assets/AssetManager.hpp"
#include "Common.hpp"

namespace Pixf::Core::Graphics {
    enum class MaterialPropertyError : uint8_t {
        None = 0,
        NotFound,
        InvalidShader,
    };

    class PIXF_API Material {
    public:
        explicit Material(Assets::AssetManager &resourceManager);
        Material(Assets::AssetManager &resourceManager, const Assets::AssetHandle &shader);

        Material(const Material &) = default;
        Material(Material &&) = delete;
        Material &operator=(const Material &) = delete;
        Material &operator=(Material &&) = delete;

        ~Material() = default;

        MaterialPropertyError SetShader(const Assets::AssetHandle &handle);
        Assets::AssetHandle GetShader() const;

        void SetDiffuse(const Math::Color4u8 &color);
        void SetDiffuseTexture2D(const std::optional<Assets::AssetHandle> &texture);
        void SetSpecular(const Math::Color4u8 &color);
        void SetSpecularTexture2D(const std::optional<Assets::AssetHandle> &texture);
        void SetSpecularStrength(float value);
        void SetShininess(float value);

        Math::Vector4f GetDiffuse() const;
        std::optional<Assets::AssetHandle> GetDiffuseTexture2D() const;
        float GetSpecularStrength() const;
        float GetShininess() const;

        void SetFloat(const std::string &name, float value);
        void SetInt(const std::string &name, int value);
        void SetVector2f(const std::string &name, const Math::Vector2f &value);
        void SetVector3f(const std::string &name, const Math::Vector3f& value);
        void SetVector4f(const std::string &name, const Math::Vector4f& value);
        void SetColor3u8(const std::string &name, const Math::Color3u8 &value);
        void SetColor4u8(const std::string &name, const Math::Color4u8 &value);
        void SetMatrix4f(const std::string &name, const Math::Matrix4f &value);

        void SetTexture2D(const std::string &name, const Assets::AssetHandle &handle);

        Error::Result<float, MaterialPropertyError> GetFloat(const std::string &name) const;
        Error::Result<int,  MaterialPropertyError> GetInt(const std::string &name) const;
        Error::Result<char, MaterialPropertyError> GetChar(const std::string &name) const;
        Error::Result<Math::Vector2f, MaterialPropertyError> GetVector2f(const std::string &name) const;
        Error::Result<Math::Vector3f, MaterialPropertyError> GetVector3f(const std::string &name) const;
        Error::Result<Math::Vector4f, MaterialPropertyError> GetVector4f(const std::string &name) const;
        Error::Result<Math::Color3u8, MaterialPropertyError> GetColor3u8(const std::string &name) const;
        Error::Result<Math::Color4u8, MaterialPropertyError> GetColor4u8(const std::string &name) const;
        Error::Result<Math::Matrix4f, MaterialPropertyError> GetMatrix4f(const std::string &name) const;

        Error::Result<Assets::AssetHandle, MaterialPropertyError> GetTexture2D(const std::string &name) const;

        void Bind() const;
        static void Unbind();

    private:
        std::unordered_map<std::string, float> m_Floats;
        std::unordered_map<std::string, int> m_Ints;
        std::unordered_map<std::string, char> m_Chars;

        std::unordered_map<std::string, Math::Vector2f> m_Vecs2;
        std::unordered_map<std::string, Math::Vector3f> m_Vecs3;
        std::unordered_map<std::string, Math::Vector4f> m_Vecs4;
        std::unordered_map<std::string, Math::Color3u8> m_Colors3;
        std::unordered_map<std::string, Math::Color4u8> m_Colors4;

        std::unordered_map<std::string, Math::Matrix4f> m_Mats;
        std::unordered_map<std::string, Assets::AssetHandle> m_Textures2D;

        Assets::AssetHandle m_Shader;
        Assets::AssetManager &m_AssetManager;

        void SetupDefaults();
    };

} // namespace Pixf::Core::Graphics

#endif // MATERIAL_HPP
