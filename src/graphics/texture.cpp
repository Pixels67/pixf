#include "texture.h"

#include <iostream>
#include <ostream>
#include <string>

#include "stb_image.h"
#include "glad/glad.h"

namespace Engine::Graphics {
    Texture::Texture(const std::string &path, TextureConfig config) {
        stbi_set_flip_vertically_on_load(1);
        int width    = 0;
        int height   = 0;
        int channels = 0;

        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load texture: " << path << '\n';
            std::exit(EXIT_FAILURE);
        }

        m_Width    = width;
        m_Height   = height;
        m_Channels = channels;

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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
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
