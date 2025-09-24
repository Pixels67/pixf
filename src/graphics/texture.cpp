#include "texture.h"

#include <iostream>
#include <ostream>
#include <string>

#include "stb_image.h"
#include "glad/glad.h"

namespace Engine::Graphics {
    Texture::Texture(const std::string &path, const TextureConfig config) : m_Path(path), m_Config(config) {
        stbi_set_flip_vertically_on_load(1);
        int width    = 0;
        int height   = 0;
        int channels = 0;

        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load texture: " << path << '\n';
            std::exit(EXIT_FAILURE);
        }

        m_Width  = width;
        m_Height = height;

        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);

        unsigned int wrapMode;
        switch (config.wrapMode) {
        case TextureConfig::WrapMode::MirroredRepeat:
            wrapMode = GL_MIRRORED_REPEAT;
            break;
        case TextureConfig::WrapMode::ClampToEdge:
            wrapMode = GL_CLAMP_TO_EDGE;
            break;
        default:
            wrapMode = GL_REPEAT;
            break;
        }

        unsigned int interpMode;
        switch (config.interpMode) {
        case TextureConfig::InterpMode::Linear:
            interpMode = GL_LINEAR;
            break;
        default:
            interpMode = GL_NEAREST;
            break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpMode);

        unsigned int format;

        switch (channels) {
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }

    Texture::Texture(const Texture &other)
    : m_Path(other.m_Path), m_Width(other.m_Width), m_Height(other.m_Height), m_Config(other.m_Config) {
        *this = std::move(Texture(other.m_Path, other.m_Config));
    }

    Texture& Texture::operator=(const Texture &other) {
        if (this == &other) return *this;

        if (m_Id != 0) {
            this->~Texture();
        }

        m_Path = other.m_Path;
        m_Config = other.m_Config;
        m_Width = other.m_Width;
        m_Height = other.m_Height;

        Texture temp(other.m_Path, other.m_Config);
        *this = std::move(temp);

        return *this;
    }

    Texture::Texture(Texture &&other) {
        if (this == &other) return;

        m_Path     = std::move(other.m_Path);
        m_Config   = other.m_Config;
        m_Width    = other.m_Width;
        m_Height   = other.m_Height;
        m_Id       = other.m_Id;
        other.m_Id = 0;
    }

    Texture &Texture::operator=(Texture &&other) {
        if (this == &other) return *this;

        if (m_Id != 0) {
            this->~Texture();
        }

        m_Path     = std::move(other.m_Path);
        m_Config   = other.m_Config;
        m_Width    = other.m_Width;
        m_Height   = other.m_Height;
        m_Id       = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_Id);
    }

    void Texture::Bind(const unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    void Texture::Unbind(const unsigned int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // namespace Engine::Graphics
