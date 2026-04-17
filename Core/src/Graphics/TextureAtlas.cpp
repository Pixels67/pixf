#include "TextureAtlas.hpp"

#include <functional>
#include <optional>
#include <utility>

#include "Graphics/Image.hpp"
#include "Graphics/Texture.hpp"
#include "Math/Rect.hpp"

namespace Flock::Graphics {
    TextureAtlas TextureAtlas::FromImage(const Image &image, const Vector2u subtextures, const TextureConfig config) {
        const Vector2u subTexSize = image.size / subtextures;
        TextureAtlas   atlas;

        atlas.m_Subtextures = subtextures;
        atlas.m_Size        = image.size;
        atlas.m_Config      = config;

        for (u32 i = 0; i < image.size.x; i += subTexSize.x) {
            for (u32 j = 0; j < image.size.y; j += subTexSize.y) {
                Image subImage = image.SubImage(Rect2u{{i, j}, subTexSize}).value();
                atlas.Insert(Vector2u{i, j}, Texture::FromImage(subImage));
            }
        }

        return atlas;
    }

    TextureAtlas TextureAtlas::CreateEmpty(const Vector2u size, const Vector2u subtextures, const TextureConfig config) {
        const Vector2u subTexSize = size / subtextures;
        TextureAtlas   atlas;

        atlas.m_Subtextures = subtextures;
        atlas.m_Size        = size;
        atlas.m_Config      = config;

        for (u32 i = 0; i < size.x; i += subTexSize.x) {
            for (u32 j = 0; i < size.y; j += subTexSize.y) {
                atlas.Insert(Vector2u{i, j}, Texture::CreateEmpty(subTexSize));
            }
        }

        return atlas;
    }

    void TextureAtlas::Configure(const TextureConfig config) {
        m_Config = config;

        for (auto &tex: m_Textures) {
            tex.Configure(config);
        }
    }

    TextureConfig TextureAtlas::Config() const {
        return m_Config;
    }

    Vector2u TextureAtlas::Size() const {
        return m_Size;
    }

    u32 TextureAtlas::Width() const {
        return Size().x;
    }

    u32 TextureAtlas::Height() const {
        return Size().y;
    }

    Texture &TextureAtlas::operator[](const Vector2u position) {
        if (!At(position)) {
            return Insert(position, Texture::Default());
        }

        return *At(position);
    }

    Texture *TextureAtlas::At(const Vector2u position) {
        if (position.y * m_Subtextures.x + position.x >= m_Textures.size()) {
            return nullptr;
        }

        return &m_Textures.at(position.y * m_Subtextures.x + position.x);
    }

    Texture &TextureAtlas::Insert(const Vector2u position, Texture &&texture) {
        while (!At(position)) {
            m_Textures.push_back(Texture::Default());
        }

        *At(position) = std::move(texture);
        return *At(position);
    }
}
