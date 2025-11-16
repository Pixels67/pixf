#include "Texture2D.hpp"

#include "Graphics/ImageData.hpp"
#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    unsigned int Texture2D::Config::GetGlWrap() const {
        switch (wrapMode) {
            case WrapMode::Clamp:
                return GL_CLAMP_TO_EDGE;
            case WrapMode::Repeat:
                return GL_REPEAT;
            case WrapMode::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            default:
                return GL_CLAMP_TO_EDGE;
        }
    }

    unsigned int Texture2D::Config::GetGlMinFilter() const {
        // May god forgive me for what I'm about to do...
        switch (filterMode) {
            case FilterMode::Linear:
                if (generateMipmaps) {
                    switch (mipmapFilterMode) {
                        case FilterMode::Linear:
                            return GL_LINEAR_MIPMAP_LINEAR;
                        case FilterMode::Nearest:
                            return GL_LINEAR_MIPMAP_NEAREST;
                    }
                }

                return GL_LINEAR;
            case FilterMode::Nearest:
                if (generateMipmaps) {
                    switch (mipmapFilterMode) {
                        case FilterMode::Linear:
                            return GL_NEAREST_MIPMAP_LINEAR;
                        case FilterMode::Nearest:
                            return GL_NEAREST_MIPMAP_NEAREST;
                    }
                }

                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    unsigned int Texture2D::Config::GetGlMagFilter() const {
        switch (filterMode) {
            case FilterMode::Linear:
                return GL_LINEAR;
            case FilterMode::Nearest:
                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    Texture2D Texture2D::Create(const ImageData &image, const Config config) {
        unsigned int id;

        PIXF_GL_CALL(glGenTextures(1, &id));
        PIXF_GL_CALL(glActiveTexture(GL_TEXTURE0));
        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.GetGlWrap()));
        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.GetGlWrap()));

        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.GetGlMinFilter()));
        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.GetGlMagFilter()));

        unsigned int format = GL_RGBA;

        if (image.GetChannels() == 1) {
            format = GL_RED;
        } else if (image.GetChannels() == 2) {
            format = GL_RG;
        } else if (image.GetChannels() == 3) {
            format = GL_RGB;
        }

        PIXF_GL_CALL(glTexImage2D(GL_TEXTURE_2D,
                                  0,
                                  format,
                                  image.GetWidth(),
                                  image.GetHeight(),
                                  0,
                                  format,
                                  GL_UNSIGNED_BYTE,
                                  image.GetPixels()));

        if (config.generateMipmaps) {
            PIXF_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
        }

        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        return Texture2D(id, config);
    }

    Texture2D::Texture2D(Texture2D &&other) noexcept : m_Id(other.m_Id), m_Config(other.m_Config) { other.m_Id = 0; }

    Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id = other.m_Id;
        m_Config = other.m_Config;
        other.m_Id = 0;

        return *this;
    }

    Texture2D::~Texture2D() { Clear(); }

    Texture2D::Config Texture2D::GetConfig() const { return m_Config; }

    void Texture2D::Bind(const unsigned int unit) const {
        PIXF_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Id));
    }

    void Texture2D::Unbind(const unsigned int unit) {
        PIXF_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    Texture2D::Texture2D(const unsigned int id, const Config config) : m_Id(id), m_Config(config) {}

    void Texture2D::Clear() const {
        if (m_Id != 0) {
            PIXF_GL_CALL(glDeleteTextures(1, &m_Id));
        }
    }
} // namespace Pixf::Core::Graphics::Gl
