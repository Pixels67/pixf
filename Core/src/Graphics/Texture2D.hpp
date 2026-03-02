#ifndef FLK_TEXTURE2D_HPP
#define FLK_TEXTURE2D_HPP

#include "Common.hpp"
#include "Graphics/Image.hpp"

namespace Flock::Graphics {
    /**
     * @enum WrapMode
     * @brief Texture wrap mode.
     */
    enum WrapMode : u8 {
        Clamp,
        Repeat,
        MirroredRepeat,
    };

    /**
     * @enum FilterMode
     * @brief Texture filter mode.
     */
    enum FilterMode : u8 {
        Nearest,
        Linear,
    };

    /**
     * @enum FilterMode
     * @brief Texture format.
     */
    enum class TextureFormat : u8 {
        R,
        Rg,
        Rgb,
        Rgba,
        Depth
    };

    inline i32 ToGlType(const TextureFormat format) {
        switch (format) {
            case TextureFormat::R:
                return GL_RED;
            case TextureFormat::Rg:
                return GL_RG;
            case TextureFormat::Rgb:
                return GL_RGB;
            case TextureFormat::Rgba:
                return GL_RGBA;
            case TextureFormat::Depth:
                return GL_DEPTH_COMPONENT;
            default:
                return 0;
        }
    }

    /**
     * @struct TextureConfig
     * @brief Texture configuration.
     */
    struct FLK_API TextureConfig {
        FilterMode                   filterMode       = Linear;
        FilterMode                   mipmapFilterMode = Linear;
        WrapMode                     wrapMode         = Repeat;
        std::optional<TextureFormat> format           = std::nullopt;
        bool                         generateMipmaps  = true;

        [[nodiscard]] u32 GetGlWrap() const;
        [[nodiscard]] u32 GetGlMinFilter() const;
        [[nodiscard]] u32 GetGlMagFilter() const;
    };

    void FLK_API ConfigureTexture2D(TextureConfig config);

    /**
     * @class Texture2D
     * @brief A 2D Texture.
     */
    class FLK_API Texture2D {
        u32           m_Id = 0;
        TextureConfig m_Config;

    public:
        /**
         * @brief Static factory method.
         * @param image The image to copy data from.
         * @param config The texture configuration.
         * @return A newly created 2D texture.
         */
        static Texture2D FromImage(const Image &image, TextureConfig config = {});

        /**
         * @brief Static factory method.
         * @param size The size of the texture.
         * @param config The texture configuration.
         * @return A newly created empty 2D texture.
         */
        static Texture2D CreateEmpty(Vector2u size, TextureConfig config = {});

        Texture2D() = default;
        ~Texture2D();

        Texture2D(const Texture2D &other) = delete;
        Texture2D(Texture2D &&other) noexcept;

        Texture2D &operator=(const Texture2D &other) = delete;
        Texture2D &operator=(Texture2D &&other) noexcept;

        /**
         * @brief Clears the texture.
         */
        void Clear() const;

        /**
         * @brief Sets the active texture unit.
         * @param unit The unit.
         */
        static void SetActiveUnit(u8 unit);

        /**
         * @brief Binds the texture to the active texture unit.
         * @return true if successful; false otherwise.
         */
        bool Bind() const;

        /**
         * @brief Unbinds a texture from the active texture unit.
         */
        static void Unbind();

        [[nodiscard]] TextureConfig GetConfig() const;

        [[nodiscard]] u32 GetGlId() const;
    };
}

#endif //FLK_TEXTURE2D_HPP
