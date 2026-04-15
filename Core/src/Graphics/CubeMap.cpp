#include "CubeMap.hpp"

#include <vector>

#include "Graphics/Gl.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Texture.hpp"
#include "Memory/Buffer.hpp"
#include "glad/glad.h"

namespace Flock::Graphics {
    std::optional<CubeMap> CubeMap::FromImages(
        const Image &       right,
        const Image &       left,
        const Image &       up,
        const Image &       down,
        const Image &       forward,
        const Image &       back,
        const TextureConfig config
    ) {
        CubeMap cubeMap;
        cubeMap.m_Config = config;

        i32 activeUnit;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &activeUnit));
        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTexture));

        FLK_GL_CALL(glGenTextures(1, &cubeMap.m_Id));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.m_Id));

        const std::vector images{right, left, down, up, forward, back};

        for (usize i = 0; i < images.size(); i++) {
            const auto &image = images[i];

            u32 format = ToGlType(image.format);
            if (config.format) {
                format = ToGlType(config.format.value());
            }

            FLK_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

            FLK_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    format,
                    image.size.x,
                    image.size.y,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    image.data.Get())
            );
        }

        ConfigureTexture(GL_TEXTURE_CUBE_MAP, config);

        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, boundTexture));

        return cubeMap;
    }

    CubeMap CubeMap::SingleColor(const Color4u8 color, const TextureConfig config) {
        const Image img = Image::SingleColor({1, 1}, color);

        return FromImages(img, img, img, img, img, img, config).value();
    }

    CubeMap::~CubeMap() {
        Clear();
    }

    CubeMap::CubeMap(CubeMap &&other) noexcept {
        m_Id       = other.m_Id;
        m_Config   = other.m_Config;
        other.m_Id = 0;
    }

    CubeMap &CubeMap::operator=(CubeMap &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        m_Config   = other.m_Config;
        other.m_Id = 0;

        return *this;
    }

    void CubeMap::Clear() const {
        if (m_Id == 0) {
            return;
        }

        FLK_GL_CALL(glDeleteTextures(1, &m_Id));
    }

    void CubeMap::SetActiveUnit(const u8 unit) {
        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
    }

    bool CubeMap::Bind() const {
        if (m_Id == 0) {
            return false;
        }

        FLK_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id));
        return true;
    }

    void CubeMap::Unbind() {
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    void CubeMap::Configure(const TextureConfig config) {
        m_Config = config;

        i32 activeUnit;
        i32 boundTexture;

        FLK_GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &activeUnit));
        FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTexture));

        FLK_GL_CALL(glActiveTexture(GL_TEXTURE0));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id));

        ConfigureTexture(GL_TEXTURE_CUBE_MAP, config);

        FLK_GL_CALL(glActiveTexture(activeUnit));
        FLK_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, boundTexture));
    }
}
