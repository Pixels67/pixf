#ifndef FLK_PIPELINE_HPP
#define FLK_PIPELINE_HPP

#include <variant>
#include <optional>
#include <string>
#include <unordered_map>

#include "Common.hpp"
#include "CubeMap.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureArray.hpp"
#include "Math/Math.hpp"

namespace Flock::Graphics {
    class CubeMap;
    class Shader;
    class TextureArray;
}

namespace Flock::Graphics {
    /**
     * @enum UniformType
     */
    enum class UniformType : u16 {
        U8,
        U32,
        I32,
        F32,
        Vec3,
        Vec2,
        Vec4,
        Col3,
        Col4,
        Mat4
    };

    using UniformData = std::variant<
        u8,
        u32,
        i32,
        f32,
        Vector3f,
        Vector2f,
        Vector4f,
        Color3u8,
        Color4u8,
        Matrix4f
    >;

    /**
     * @struct Uniform
     * @brief A Pipeline uniform.
     */
    struct FLK_API Uniform {
        UniformType type;
        UniformData data;
    };

    struct SamplerInfo {
        i32 unit;
        u32 glType;
    };

    /**
     * @class Pipeline
     * @brief A shader pipeline.
     */
    class FLK_API Pipeline {
        u32                                          m_Id = 0;
        std::unordered_map<std::string, Uniform>     m_Uniforms;
        std::unordered_map<std::string, SamplerInfo> m_Samplers;
        Texture                                      m_DefaultTexture;

    public:
        /**
         * @brief Static factory method.
         * @param vertex The vertex shader.
         * @param fragment The fragment shader.
         * @return The pipeline if successful; std::nullopt otherwise.
         */
        static std::optional<Pipeline> Create(const Shader &vertex, const Shader &fragment);

        Pipeline() = default;
        ~Pipeline();

        Pipeline(const Pipeline &other) = delete;
        Pipeline(Pipeline &&other) noexcept;

        Pipeline &operator=(const Pipeline &other) = delete;
        Pipeline &operator=(Pipeline &&other) noexcept;

        /**
         * @brief Clears the shader.
         */
        void Clear() const;

        /**
         * @brief Binds the shader pipeline.
         * @return true if successful; false otherwise.
         */
        bool Bind() const;

        /**
         * @brief Unbinds the shader pipeline.
         */
        static void Unbind();

        /**
         * @brief Sets a byte uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, u8 value);

        /**
         * @brief Sets an unsigned 32-bit integer uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, u32 value);

        /**
         * @brief Sets a signed 32-bit integer uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, i32 value);

        /**
         * @brief Sets a 32-bit floating-point uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, f32 value);

        /**
         * @brief Sets a 2D vector uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, Vector2f value);

        /**
         * @brief Sets a 3D vector uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, Vector3f value);

        /**
         * @brief Sets a 4D vector uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, Vector4f value);

        /**
         * @brief Sets an RGB color uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, Color3u8 value);

        /**
         * @brief Sets an RGBA color uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, Color4u8 value);

        /**
         * @brief Sets a 4x4 matrix uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         */
        void SetUniform(const std::string &name, Matrix4f value);

        /**
         * @brief Sets a 2D texture (sampler2D) uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         * @return true if successful; false otherwise.
         */
        bool SetUniform(const std::string &name, const Texture &value) const;

        /**
         * @brief Sets a cube map (samplerCube) uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         * @return true if successful; false otherwise.
         */
        bool SetUniform(const std::string &name, const CubeMap &value) const;

        /**
         * @brief Sets a 2D texture array (sampler2DArray) uniform.
         * @param name The uniform name.
         * @param value The uniform value to set.
         * @return true if successful; false otherwise.
         */
        bool SetUniform(const std::string &name, const TextureArray &value) const;

        /**
         * @brief Resets all uniforms;
         */
        void ResetUniforms();

    private:
        static u32 LinkShaders(const Shader &vertex, const Shader &fragment);

        bool SetSamplerUnits();
        bool BindUniform(const std::string &name, const Uniform &uniform) const;

        void SetDefaultTextures(bool overwrite = false) const;
    };
}

#endif //FLK_PIPELINE_HPP
