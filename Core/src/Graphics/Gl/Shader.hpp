#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include "../../Common.hpp"

constexpr unsigned int maxTextureCount = 16;
constexpr unsigned int maxTextureNameLength = 256;

namespace Pixf::Core::Graphics::Gl {
    enum class ShaderError : uint8_t {
        VertShaderCompileFailed,
        FragShaderCompileFailed,
        ShaderCompileFailed,
    };

    class Shader {
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

        void SetUniform(const std::string &name, vec2 value) const;
        void SetUniform(const std::string &name, vec3 value) const;
        void SetUniform(const std::string &name, vec4 value) const;

        void SetUniform(const std::string &name, mat4 value) const;

        std::unordered_map<std::string, uint8_t> GetTextureMap() const;

    private:
        unsigned int m_Id;
        std::unordered_map<std::string, uint8_t> m_TextureMap;

        static unsigned int CreateGlShader(unsigned int type, const std::string &src);
        static unsigned int LinkGlShaders(unsigned int vertShader, unsigned int fragShader);

        void Cleanup() const;

        void InitTextureMap();
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // SHADER_HPP
