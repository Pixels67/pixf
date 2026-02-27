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
     * @struct TextureConfig
     * @brief Texture configuration.
     */
    struct FLK_API TextureConfig {
        FilterMode filterMode       = Linear;
        FilterMode mipmapFilterMode = Linear;
        WrapMode   wrapMode         = Repeat;
        bool       generateMipmaps  = false;

        [[nodiscard]] u32 GetGlWrap() const;
        [[nodiscard]] u32 GetGlMinFilter() const;
        [[nodiscard]] u32 GetGlMagFilter() const;
    };

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
        static Texture2D Create(const Image &image, TextureConfig config = {});

        Texture2D() = default;
        ~Texture2D();

        Texture2D(const Texture2D &other)     = delete;
        Texture2D(Texture2D &&other) noexcept;

        Texture2D &operator=(const Texture2D &other)     = delete;
        Texture2D &operator=(Texture2D &&other) noexcept;

        /**
         * @brief Clears the texture.
         */
        void Clear() const;

        /**
         * @brief Binds the texture to a texture unit.
         * @param unit The texture unit to bind to.
         * @return true if successful; false otherwise.
         */
        bool Bind(u8 unit) const;

        /**
         * @brief Unbinds a texture from a texture unit.
         * @param unit The texture unit to unbind from.
         */
        static void Unbind(u8 unit);

        [[nodiscard]] TextureConfig GetConfig() const;
    };
}

#endif //FLK_TEXTURE2D_HPP
