#ifndef PIXF_MATERIAL_HPP
#define PIXF_MATERIAL_HPP

#include "Error/Error.hpp"
#include "Handle.hpp"
#include "Math/Color.hpp"
#include "Math/Vector.hpp"

namespace Pixf::Core::Graphics {
    enum class UniformType : uint8_t { Float = 0, Int, Char, Vector2, Vector3, Vector4, Color3, Color4, Texture2D };

    class PIXF_API MaterialUniformError final : public Error::Error {
        using Error::Error;
    };

    class PIXF_API Material {
    public:
        void SetShader(ShaderHandle shader);
        void SetFloat(const std::string &name, float value);
        void SetInt(const std::string &name, int value);
        void SetChar(const std::string &name, char value);
        void SetVector2(const std::string &name, Math::Vector2f value);
        void SetVector3(const std::string &name, Math::Vector3f value);
        void SetVector4(const std::string &name, const Math::Vector4f &value);
        void SetColor3(const std::string &name, Math::Color3u8 value);
        void SetColor4(const std::string &name, Math::Color4u8 value);
        void SetTexture2D(const std::string &name, Texture2DHandle value);

        ShaderHandle GetShader() const;
        std::optional<float> GetFloat(const std::string &name) const;
        std::optional<int> GetInt(const std::string &name) const;
        std::optional<char> GetChar(const std::string &name) const;
        std::optional<Math::Vector2f> GetVector2(const std::string &name) const;
        std::optional<Math::Vector3f> GetVector3(const std::string &name) const;
        std::optional<Math::Vector4f> GetVector4(const std::string &name) const;
        std::optional<Math::Color3u8> GetColor3(const std::string &name) const;
        std::optional<Math::Color4u8> GetColor4(const std::string &name) const;
        std::optional<Texture2DHandle> GetTexture2D(const std::string &name) const;

    private:
        ShaderHandle m_Shader;
        size_t m_UniformCount = 0;
        std::vector<std::string> m_Names;
        std::vector<UniformType> m_Types;
        std::vector<std::optional<float>> m_Floats;
        std::vector<std::optional<int>> m_Ints;
        std::vector<std::optional<char>> m_Chars;
        std::vector<std::optional<Math::Vector2f>> m_Vector2s;
        std::vector<std::optional<Math::Vector3f>> m_Vector3s;
        std::vector<std::optional<Math::Vector4f>> m_Vector4s;
        std::vector<std::optional<Math::Color3u8>> m_Color3s;
        std::vector<std::optional<Math::Color4u8>> m_Color4s;
        std::vector<std::optional<Texture2DHandle>> m_Textures;

        std::optional<size_t> FindUniform(const std::string &name) const;

        template<typename T>
        void SetUniform(const std::string &name,
                        const UniformType type,
                        const T &value,
                        std::vector<std::optional<T>> &storage) {
            if (auto idx = FindUniform(name)) {
                if (m_Types[*idx] == type) {
                    storage[*idx] = value;
                }

                throw MaterialUniformError("Failed to set uniform: Type mismatch");
            }

            m_Names.push_back(name);
            m_Types.push_back(type);

            m_Floats.resize(m_Names.size());
            m_Ints.resize(m_Names.size());
            m_Chars.resize(m_Names.size());
            m_Vector2s.resize(m_Names.size());
            m_Vector3s.resize(m_Names.size());
            m_Vector4s.resize(m_Names.size());
            m_Color3s.resize(m_Names.size());
            m_Color4s.resize(m_Names.size());
            m_Textures.resize(m_Names.size());

            storage.back() = value;
            m_UniformCount++;
        }

        template<typename T>
        std::optional<T> GetUniform(const std::string &name,
                                    const UniformType expectedType,
                                    const std::vector<std::optional<T>> &storage) const {
            if (auto idx = FindUniform(name)) {
                if (m_Types[*idx] == expectedType) {
                    return storage[*idx];
                }
            }

            return std::nullopt;
        }
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_MATERIAL_HPP
