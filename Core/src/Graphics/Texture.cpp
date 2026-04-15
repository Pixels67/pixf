#include "Texture.hpp"

#include "Gl.hpp"
#include "Graphics/Image.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::Graphics {
    u32 TextureConfig::GlWrap() const {
        switch (wrapMode) {
            case Clamp:
                return GL_CLAMP_TO_EDGE;
            case Repeat:
                return GL_REPEAT;
            case MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            default:
                return GL_CLAMP_TO_EDGE;
        }
    }

    u32 TextureConfig::GlMinFilter() const {
        // May god forgive me for what I'm about to do...
        switch (filterMode) {
            case Linear:
                if (generateMipmaps) {
                    switch (mipmapFilterMode) {
                        case Linear:
                            return GL_LINEAR_MIPMAP_LINEAR;
                        case Nearest:
                            return GL_LINEAR_MIPMAP_NEAREST;
                    }
                }

                return GL_LINEAR;
            case Nearest:
                if (generateMipmaps) {
                    switch (mipmapFilterMode) {
                        case Linear:
                            return GL_NEAREST_MIPMAP_LINEAR;
                        case Nearest:
                            return GL_NEAREST_MIPMAP_NEAREST;
                    }
                }

                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    u32 TextureConfig::GlMagFilter() const {
        switch (filterMode) {
            case Linear:
                return GL_LINEAR;
            case Nearest:
                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    void ConfigureTexture(const u32 glTexture, const TextureConfig config) {
        FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_WRAP_S, config.GlWrap()));
        FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_WRAP_T, config.GlWrap()));

        FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_MIN_FILTER, config.GlMinFilter()));
        FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_MAG_FILTER, config.GlMagFilter()));

        if (config.generateMipmaps && (!config.format || config.format.value() != TextureFormat::Depth)) {
            FLK_GL_CALL(glGenerateMipmap(glTexture));
        }

        if (config.format && config.format.value() == TextureFormat::Depth) {
            FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
            FLK_GL_CALL(glTexParameteri(glTexture, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
        }
    }

    Texture Texture::FromImage(const Image &image, const TextureConfig config) {
        Texture texture;
        texture.m_Config = config;
        texture.m_Size   = image.size;

        i32 activeUnit;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &activeUnit));
        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture));

        FLK_GL_CALL(glGenTextures(1, &texture.m_Id));
        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.m_Id));

        u32 format = ToGlType(image.format);
        if (config.format) {
            format = ToGlType(config.format.value());
        }

        const Image flipped = image.FlipY();

        FLK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

        FLK_GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                0,
                format,
                image.size.x,
                image.size.y,
                0,
                ToGlType(image.format),
                GL_UNSIGNED_BYTE,
                flipped.data.Get())
        );

        ConfigureTexture(GL_TEXTURE_2D, config);

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, boundTexture));

        return texture;
    }

    Texture Texture::CreateEmpty(const Vector2u size, const TextureConfig config) {
        Texture texture;
        texture.m_Config = config;
        texture.m_Size   = size;

        i32 activeUnit;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &activeUnit));
        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture));

        FLK_GL_CALL(glGenTextures(1, &texture.m_Id));
        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.m_Id));

        u32 varType = GL_UNSIGNED_BYTE;
        u32 format  = GL_RGBA;
        if (config.format) {
            format = ToGlType(config.format.value());
            if (config.format.value() == TextureFormat::Depth) {
                varType = GL_FLOAT;
            }
        }

        FLK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

        FLK_GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                0,
                format,
                size.x,
                size.y,
                0,
                format,
                varType,
                nullptr)
        );

        ConfigureTexture(GL_TEXTURE_2D, config);

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, boundTexture));

        return texture;
    }

    Texture Texture::Default(const TextureConfig config) {
        return FromImage(Image::Default(), config);
    }

    Texture::~Texture() {
        Clear();
    }

    Texture::Texture(Texture &&other) noexcept {
        m_Id       = other.m_Id;
        m_Config   = other.m_Config;
        other.m_Id = 0;
    }

    Texture &Texture::operator=(Texture &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        m_Config   = other.m_Config;
        other.m_Id = 0;

        return *this;
    }

    void Texture::Clear() const {
        if (m_Id != 0) {
            FLK_GL_CALL(glDeleteTextures(1, &m_Id));
        }
    }

    void Texture::SetActiveUnit(const u8 unit) {
        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
    }

    bool Texture::Bind() const {
        if (m_Id == 0) {
            return false;
        }

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Id));

        return true;
    }

    void Texture::Unbind() {
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Texture::Configure(const TextureConfig config) {
        m_Config = config;

        i32 activeUnit;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &activeUnit));
        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture));

        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Id));

        ConfigureTexture(GL_TEXTURE_2D, config);

        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, boundTexture));
    }

    TextureConfig Texture::Config() const {
        return m_Config;
    }

    Vector2u Texture::Size() const {
        return m_Size;
    }

    u32 Texture::Width() const {
        return Size().x;
    }

    u32 Texture::Height() const {
        return Size().y;
    }

    u32 Texture::GlId() const {
        return m_Id;
    }
}
