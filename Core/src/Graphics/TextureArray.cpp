#include "TextureArray.hpp"

namespace Flock::Graphics {
    void ConfigureTextureArray(TextureConfig config) {
        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, config.GetGlWrap()));
        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, config.GetGlWrap()));

        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, config.GetGlMinFilter()));
        FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, config.GetGlMagFilter()));

        if (config.generateMipmaps && (!config.format || config.format.value() != TextureFormat::Depth)) {
            FLK_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D_ARRAY));
        }

        if (config.format && config.format.value() == TextureFormat::Depth) {
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
            FLK_GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
        }
    }

    TextureArray TextureArray::Create(const usize layers, const Vector2u size, const TextureConfig config) {
        TextureArray texArr;
        texArr.m_Layers = layers;
        texArr.m_Config = config;
        texArr.m_Size   = size;

        glGenTextures(1, &texArr.m_Id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texArr.m_Id);

        u32 varType = GL_UNSIGNED_BYTE;
        u32 format  = GL_RGBA;
        if (config.format) {
            format = ToGlType(config.format.value());
            if (config.format.value() == TextureFormat::Depth) {
                varType = GL_FLOAT;
            }
        }

        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            GL_DEPTH_COMPONENT32F,
            size.x,
            size.y,
            layers,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            nullptr
        );

        ConfigureTextureArray(config);

        return texArr;
    }

    TextureArray::~TextureArray() {
        Clear();
    }

    TextureArray::TextureArray(TextureArray &&other) noexcept {
        m_Id       = other.m_Id;
        m_Layers   = other.m_Layers;
        m_Config   = other.m_Config;
        m_Size     = other.m_Size;
        other.m_Id = 0;
    }

    TextureArray &TextureArray::operator=(TextureArray &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        m_Layers   = other.m_Layers;
        m_Config   = other.m_Config;
        m_Size     = other.m_Size;
        other.m_Id = 0;

        return *this;
    }

    void TextureArray::Clear() const {
        if (m_Id == 0) {
            return;
        }

        glDeleteTextures(1, &m_Id);
    }

    void TextureArray::SetActiveUnit(u8 unit) {
        return Texture::SetActiveUnit(unit);
    }

    bool TextureArray::Bind() const {
        if (m_Id == 0 || m_Layers == 0) {
            return false;
        }

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, m_Id));

        return true;
    }

    void TextureArray::Unbind() {
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
    }

    Vector2u TextureArray::GetSize() const {
        return m_Size;
    }

    u32 TextureArray::GetGlId() const {
        return m_Id;
    }

    u32 TextureArray::GetLayerCount() const {
        return m_Layers;
    }
}
