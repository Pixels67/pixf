#ifndef FLK_TEXTUREARRAY_HPP
#define FLK_TEXTUREARRAY_HPP

#include "Common.hpp"
#include "Texture.hpp"
#include "Math/Vector.hpp"

namespace Flock::Graphics {
    void FLK_API ConfigureTextureArray(TextureConfig config);

    class FLK_API TextureArray {
        u32           m_Id     = 0;
        u32           m_Layers = 0;
        TextureConfig m_Config = {};
        Vector2u      m_Size   = {};

    public:
        static TextureArray Create(usize layers, Vector2u size, TextureConfig config = {});

        TextureArray() = default;
        ~TextureArray();

        TextureArray(const TextureArray &other) = delete;
        TextureArray(TextureArray &&other) noexcept;

        TextureArray &operator=(const TextureArray &other) = delete;
        TextureArray &operator=(TextureArray &&other) noexcept;

        /**
         * @brief Clears the texture array.
         */
        void Clear() const;

        /**
         * @brief Sets the active texture unit.
         * @param unit The unit.
         */
        static void SetActiveUnit(u8 unit);

        /**
         * @brief Binds the texture array to the active texture unit.
         * @return true if successful; false otherwise.
         */
        bool Bind() const;

        /**
         * @brief Unbinds a texture array from the active texture unit.
         */
        static void Unbind();

        [[nodiscard]] Vector2u GetSize() const;
        [[nodiscard]] u32      GetGlId() const;
        [[nodiscard]] u32      GetLayerCount() const;
    };
}

#endif //FLK_TEXTUREARRAY_HPP
