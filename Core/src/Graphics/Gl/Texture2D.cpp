#include "Texture2D.hpp"

#include "Debug/Logger.hpp"
#include "Gl.hpp"
#include "stb_image.h"

namespace Pixf::Core::Graphics::Gl {
    unsigned int TextureConfig::GetGlWrap() const {
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

    unsigned int TextureConfig::GetGlMinFilter() const {
        switch (filterMode) {
            case FilterMode::Linear:
                if (generateMipmaps) {
                    return GL_LINEAR_MIPMAP_LINEAR;
                }

                return GL_LINEAR;
            case FilterMode::Nearest:
                if (generateMipmaps) {
                    return GL_NEAREST_MIPMAP_NEAREST;
                }

                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    unsigned int TextureConfig::GetGlMagFilter() const {
        switch (filterMode) {
            case FilterMode::Linear:
                return GL_LINEAR;
            case FilterMode::Nearest:
                return GL_NEAREST;
            default:
                return GL_LINEAR;
        }
    }

    bool TextureConfig::operator==(const TextureConfig &other) const {
        return filterMode == other.filterMode && mipmapFilterMode == other.mipmapFilterMode &&
               wrapMode == other.wrapMode && generateMipmaps == other.generateMipmaps;
    }

    bool TextureConfig::operator!=(const TextureConfig &other) const { return !(*this == other); }

    Error::Result<Texture2D, TextureError> Texture2D::Load(const std::string &path, const TextureConfig config) {
        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(1);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (data == nullptr) {
            return TextureError::FailedToLoad;
        }

        unsigned int id;

        PIXF_GL_CALL(glGenTextures(1, &id));
        PIXF_GL_CALL(glActiveTexture(GL_TEXTURE0));
        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.GetGlWrap()));
        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.GetGlWrap()));

        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.GetGlMinFilter()));
        PIXF_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.GetGlMagFilter()));

        const unsigned int format = channels == 3 ? GL_RGB : GL_RGBA;
        PIXF_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));

        if (config.generateMipmaps) {
            PIXF_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
        }

        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        stbi_image_free(data);

        return Error::Result<Texture2D, TextureError>(Texture2D(id, config));
    }

    Texture2D::Texture2D(Texture2D &&other) noexcept : m_Id(other.m_Id), m_Config(other.m_Config) { other.m_Id = 0; }

    Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Id = other.m_Id;
        m_Config = other.m_Config;
        other.m_Id = 0;

        return *this;
    }

    Texture2D::~Texture2D() {
        Cleanup();
    }

    void Texture2D::Bind(const unsigned int index) const {
        PIXF_GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Id));
    }

    void Texture2D::Unbind(const unsigned int index) {
        PIXF_GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
        PIXF_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    TextureConfig Texture2D::GetConfig() const { return m_Config; }

    Texture2D::Texture2D(const unsigned int id, const TextureConfig config) : m_Id(id), m_Config(config) {}

    void Texture2D::Cleanup() const { PIXF_GL_CALL(glDeleteTextures(1, &m_Id)); }
} // namespace Pixf::Core::Graphics::Gl
