#ifndef FLK_IMAGE_HPP
#define FLK_IMAGE_HPP

#include "Math/Math.hpp"
#include "Common.hpp"
#include "Gl.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::Graphics {
    enum class ImageFormat : u8 {
        R,
        Rg,
        Rgb,
        Rgba
    };

    inline u32 ToGlType(const ImageFormat format) {
        switch (format) {
            case ImageFormat::R:
                return GL_RED;
            case ImageFormat::Rg:
                return GL_RG;
            case ImageFormat::Rgb:
                return GL_RGB;
            case ImageFormat::Rgba:
                return GL_RGBA;
            default:
                return 0;
        }
    }

    /**
     * @struct Image
     * @brief 2D image data.
     */
    struct FLK_API Image {
        Memory::Buffer data;
        Vector2u       size;
        ImageFormat    format;

        static Image Default() {
            const std::vector<u8> bytes{255, 255, 255, 255};

            return {
                .data   = Memory::Buffer(bytes.data(), bytes.size()),
                .size   = {1, 1},
                .format = ImageFormat::Rgba,
            };
        }

        static Image SingleColor(const Vector2u size, const Color4u8 color) {
            const u32 packed = (static_cast<u32>(color.a) << 24 |
                                static_cast<u32>(color.b) << 16 |
                                static_cast<u32>(color.g) << 8 |
                                static_cast<u32>(color.r));

            const std::vector bytes(size.x * size.y, packed);

            return {
                .data   = Memory::Buffer(bytes.data(), bytes.size() * 4),
                .size   = size,
                .format = ImageFormat::Rgba,
            };
        }
    };
}

#endif //FLK_IMAGE_HPP
