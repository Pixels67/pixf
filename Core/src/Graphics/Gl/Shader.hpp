#ifndef PIXF_SHADER_HPP
#define PIXF_SHADER_HPP

#include "Common.hpp"
#include "Error/Error.hpp"
#include "Math/Color.hpp"
#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"
#include "Texture2D.hpp"

namespace Pixf::Core::Graphics::Gl {
    class ShaderCompilationError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API Shader {
    public:
        static Shader Create(const std::string &vertSrc, const std::string &fragSrc);

        Shader() = default;

        Shader(const Shader &other) = delete;
        Shader(Shader &&other) noexcept;

        Shader &operator=(const Shader &other) = delete;
        Shader &operator=(Shader &&other) noexcept;

        ~Shader();

        void Bind();
        static void Unbind();

        void SetUniform(const std::string &name, char value);
        void SetUniform(const std::string &name, int value);
        void SetUniform(const std::string &name, float value);
        void SetUniform(const std::string &name, const Math::Vector2f &value);
        void SetUniform(const std::string &name, const Math::Vector3f &value);
        void SetUniform(const std::string &name, const Math::Vector4f &value);
        void SetUniform(const std::string &name, Math::Color3u8 value);
        void SetUniform(const std::string &name, Math::Color4u8 value);
        void SetUniform(const std::string &name, const Math::Matrix4f &value);
        void SetUniform(const std::string & name, const Texture2D &value);

        std::unordered_map<std::string, uint8_t> GetTextureUniformMap() const;

        void Clear() const;

    private:
        unsigned int m_Id = 0;
        std::unordered_map<std::string, unsigned int> m_UniformLocations;
        std::unordered_map<std::string, uint8_t> m_TextureUniformMap;

        explicit Shader(unsigned int id);

        static unsigned int CreateGlShader(unsigned int type, const std::string &src);
        static unsigned int LinkGlShaders(unsigned int vertShader, unsigned int fragShader);
        static unsigned int GetActiveShader();

        void InitTextureUniformMap();
        unsigned int GetUniformCount() const;
        std::string GetUniformName(unsigned int uniformId) const;
        unsigned int GetUniformType(unsigned int uniformId) const;
        static Texture2D &GetDefaultTexture();
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_SHADER_HPP
