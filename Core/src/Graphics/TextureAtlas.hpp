#ifndef TEXTUREATLAS_HPP
#define TEXTUREATLAS_HPP

#include <vector>

#include "Texture.hpp"
#include "Common.hpp"
#include "Math/Vector.hpp"

namespace Flock {
namespace Graphics {
struct Image;
}  // namespace Graphics
}  // namespace Flock

namespace Flock::Graphics {
    class FLK_API TextureAtlas {
        std::vector<Texture> m_Textures;
        TextureConfig        m_Config;
        Vector2u             m_Size        = {};
        Vector2u             m_Subtextures = {};

    public:
        /**
         * @brief Static factory method.
         * @param image The image to copy data from.
         * @param subtextures The number of subtextures (horizontal and vertical) the atlas has.
         * @param config The texture configuration.
         * @return A newly created texture atlas.
         */
        static TextureAtlas FromImage(const Image &image, Vector2u subtextures, TextureConfig config = {});

        /**
         * @brief Static factory method.
         * @param size The size of the texture.
         * @param subtextures The number of subtextures (horizontal and vertical) the atlas has.
         * @param config The texture configuration.
         * @return A newly created texture atlas.
         */
        static TextureAtlas CreateEmpty(Vector2u size, Vector2u subtextures, TextureConfig config = {});

        TextureAtlas()  = default;
        ~TextureAtlas() = default;

        TextureAtlas(const TextureAtlas &other)     = delete;
        TextureAtlas(TextureAtlas &&other) noexcept = default;

        TextureAtlas &operator=(const TextureAtlas &other)     = delete;
        TextureAtlas &operator=(TextureAtlas &&other) noexcept = default;

        void Configure(TextureConfig config);

        [[nodiscard]] TextureConfig Config() const;

        [[nodiscard]] Vector2u Size() const;
        [[nodiscard]] u32      Width() const;
        [[nodiscard]] u32      Height() const;

        [[nodiscard]] Texture &            operator[](Vector2u position);
        [[nodiscard]] OptionalRef<Texture> At(Vector2u position);

        Texture &Insert(Vector2u position, Texture &&texture);
    };
}

#endif //TEXTUREATLAS_HPP
