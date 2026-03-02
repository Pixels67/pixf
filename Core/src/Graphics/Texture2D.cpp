#include "Texture2D.hpp"

#include "Gl.hpp"

namespace Flock::Graphics {
    u32 TextureConfig::GetGlWrap() const {
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

    u32 TextureConfig::GetGlMinFilter() const {
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

    u32 TextureConfig::GetGlMagFilter() const {
        switch (filterMode) {
            case Linear:
                return GL_LINEAR;
            case Nearest:
                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    void ConfigureTexture2D(const TextureConfig config) {
        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.GetGlWrap()));
        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.GetGlWrap()));

        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.GetGlMinFilter()));
        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.GetGlMagFilter()));

        if (config.generateMipmaps && (!config.format || config.format.value() != TextureFormat::Depth)) {
            FLK_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
        }

        if (config.format && config.format.value() == TextureFormat::Depth) {
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
        }
    }

    Texture2D Texture2D::FromImage(const Image &image, const TextureConfig config) {
        Texture2D texture;
        texture.m_Config = config;

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

        FLK_GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                0,
                format,
                image.size.x,
                image.size.y,
                0,
                format,
                GL_UNSIGNED_BYTE,
                image.data.Get())
        );

        ConfigureTexture2D(config);

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, boundTexture));

        return texture;
    }

    Texture2D Texture2D::CreateEmpty(const Vector2u size, TextureConfig config) {
        Texture2D texture;
        texture.m_Config = config;

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
                varType                = GL_FLOAT;
            }
        }

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

        ConfigureTexture2D(config);

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, boundTexture));

        return texture;
    }

    Texture2D::~Texture2D() {
        Clear();
    }

    Texture2D::Texture2D(Texture2D &&other) noexcept {
        m_Id       = other.m_Id;
        m_Config   = other.m_Config;
        other.m_Id = 0;
    }

    Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        m_Config   = other.m_Config;
        other.m_Id = 0;

        return *this;
    }

    void Texture2D::Clear() const {
        if (m_Id != 0) {
            FLK_GL_CALL(glDeleteTextures(1, &m_Id));
        }
    }

    void Texture2D::SetActiveUnit(const u8 unit) {
        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
    }

    bool Texture2D::Bind() const {
        if (m_Id == 0) {
            return false;
        }

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Id));

        return true;
    }

    void Texture2D::Unbind() {
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    TextureConfig Texture2D::GetConfig() const {
        return m_Config;
    }

    u32 Texture2D::GetGlId() const {
        return m_Id;
    }
}
