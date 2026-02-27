#ifndef FLK_SHADER_HPP
#define FLK_SHADER_HPP

#include "Common.hpp"

namespace Flock::Graphics {
    enum ShaderType : u8 {
        VertexShader,
        FragmentShader
    };

    u32 ToGlType(ShaderType shaderType);

    /**
     * @class Shader
     * @brief An OpenGL shader.
     */
    class FLK_API Shader {
        u32 m_Id = 0;

    public:
        /**
         * @brief Static factory method.
         * @param type The shader type.
         * @param source The shader source code.
         * @return The shader if successful; std::nullopt otherwise.
         */
        static std::optional<Shader> Create(ShaderType type, const std::string &source);

        Shader() = default;
        ~Shader();

        Shader(const Shader &other) = delete;
        Shader(Shader &&other) noexcept;

        Shader &operator=(const Shader &other) = delete;
        Shader &operator=(Shader &&other) noexcept;

        /**
         * @brief Clears the shader.
         */
        void Clear() const;

        [[nodiscard]] u32 GetGlId() const;
    };
}

#endif // FLK_SHADER_HPP
