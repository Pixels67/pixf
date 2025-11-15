#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include "Math/Math.hpp"

constexpr unsigned int g_MaxTextureCount = 16;
constexpr unsigned int maxTextureNameLength = 256;

namespace Pixf::Core::Graphics::Gl {
    enum class ShaderError : uint8_t {
        VertShaderCompileFailed,
        FragShaderCompileFailed,
        ShaderCompileFailed,
    };

    class PIXF_API Shader {
    public:
        Shader(const std::string &vertSrc, const std::string &fragSrc);

        Shader(const Shader &other) = delete;
        Shader(Shader &&other) noexcept;

        Shader &operator=(const Shader &other) = delete;
        Shader &operator=(Shader &&other) noexcept;

        ~Shader();

        void Bind() const;
        static void Unbind();

        void SetUniform(const std::string &name, char value) const;
        void SetUniform(const std::string &name, int value) const;
        void SetUniform(const std::string &name, float value) const;

        void SetUniform(const std::string &name, const Math::Vector2f& value) const;
        void SetUniform(const std::string &name, const Math::Vector3f &value) const;
        void SetUniform(const std::string &name, const Math::Vector4f &value) const;
        void SetUniform(const std::string &name, Math::Color3u8 value) const;
        void SetUniform(const std::string &name, Math::Color4u8 value) const;

        void SetUniform(const std::string &name, Math::Matrix4f value) const;

        std::unordered_map<std::string, uint8_t> GetTextureMap() const;

        void Cleanup() const;

    private:
        unsigned int m_Id;
        std::unordered_map<std::string, uint8_t> m_TextureMap;

        static unsigned int CreateGlShader(unsigned int type, const std::string &src);
        static unsigned int LinkGlShaders(unsigned int vertShader, unsigned int fragShader);

        void InitTextureMap();
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // SHADER_HPP
