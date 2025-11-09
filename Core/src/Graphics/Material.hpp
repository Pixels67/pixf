#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <unordered_map>
#include <optional>

#include "../Assets/AssetManager.hpp"
#include "../Common.hpp"

namespace Pixf::Core::Graphics {
    enum class MaterialPropertyError : uint8_t {
        None = 0,
        NotFound,
        InvalidShader,
    };

    class Material {
    public:
        explicit Material(AssetManager &resourceManager);
        Material(AssetManager &resourceManager, AssetHandle shader);

        Material(const Material &) = default;
        Material(Material &&) = delete;
        Material &operator=(const Material &) = delete;
        Material &operator=(Material &&) = delete;

        ~Material() = default;

        MaterialPropertyError SetShader(AssetHandle handle);
        AssetHandle GetShader() const;

        void SetDiffuse(vec4 color);
        void SetDiffuseTexture2D(std::optional<AssetHandle> texture);
        void SetSpecular(vec4 color);
        void SetSpecularTexture2D(std::optional<AssetHandle> texture);
        void SetSpecularStrength(float value);
        void SetShininess(float value);

        vec4 GetDiffuse();
        std::optional<AssetHandle> GetDiffuseTexture2D();
        float GetSpecularStrength();
        float GetShininess();

        MaterialPropertyError SetFloat(const std::string &name, float value);
        MaterialPropertyError SetInt(const std::string &name, int value);
        MaterialPropertyError SetVec2(const std::string &name, vec2 value);
        MaterialPropertyError SetVec3(const std::string &name, vec3 value);
        MaterialPropertyError SetVec4(const std::string &name, vec4 value);
        MaterialPropertyError SetMat4(const std::string &name, const mat4 &value);

        MaterialPropertyError SetTexture2D(const std::string &name, AssetHandle handle);

        Error::Result<float, MaterialPropertyError> GetFloat(const std::string &name);
        Error::Result<int,  MaterialPropertyError> GetInt(const std::string &name);
        Error::Result<char, MaterialPropertyError> GetChar(const std::string &name);
        Error::Result<vec2, MaterialPropertyError> GetVec2(const std::string &name);
        Error::Result<vec3, MaterialPropertyError> GetVec3(const std::string &name);
        Error::Result<vec4, MaterialPropertyError> GetVec4(const std::string &name);
        Error::Result<mat4, MaterialPropertyError> GetMat4(const std::string &name);

        Error::Result<AssetHandle, MaterialPropertyError> GetTexture2D(const std::string &name);

        void Bind() const;
        static void Unbind();

    private:
        std::unordered_map<std::string, float> m_Floats;
        std::unordered_map<std::string, int> m_Ints;
        std::unordered_map<std::string, char> m_Chars;

        std::unordered_map<std::string, vec2> m_Vecs2D;
        std::unordered_map<std::string, vec3> m_Vecs3D;
        std::unordered_map<std::string, vec4> m_Vecs4D;

        std::unordered_map<std::string, mat4> m_Mats;
        std::unordered_map<std::string, AssetHandle> m_Textures2D;

        AssetHandle m_Shader;
        AssetManager &m_ResourceManager;

        void SetupDefaults();
    };

} // namespace Pixf::Core::Graphics

#endif // MATERIAL_HPP
