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

    inline u8 ChannelCount(const ImageFormat format) {
        switch (format) {
            case ImageFormat::R:
                return 1;
            case ImageFormat::Rg:
                return 2;
            case ImageFormat::Rgb:
                return 3;
            case ImageFormat::Rgba:
                return 4;
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

        [[nodiscard]] std::vector<u8> PixelAt(const u32 x, const u32 y) const {
            std::vector<u8> vec;

            const u8 *pixel = static_cast<u8 *>(data.Get()) + (y * size.x + x) * ChannelCount(format);
            vec.reserve(ChannelCount(format));
            for (usize c = 0; c < ChannelCount(format); c++) {
                vec.push_back(pixel[c]);
            }

            return vec;
        }

        [[nodiscard]] std::optional<Image> SubImage(const Rect2u rect) const {
            if (rect.origin.x + rect.origin.x > size.x) {
                return std::nullopt;
            }

            if (rect.origin.y + rect.origin.y > size.y) {
                return std::nullopt;
            }

            const auto      vec = data.Vector<u8>();
            std::vector<u8> subVec;

            for (usize j = rect.origin.y; j < rect.origin.y + rect.aspect.y; j++) {
                for (usize i = rect.origin.x; i < rect.origin.x + rect.aspect.x; i++) {
                    for (auto &c: PixelAt(i, j)) {
                        subVec.push_back(c);
                    }
                }
            }

            return Image{.data = subVec, .size = rect.aspect, .format = format};
        }

        [[nodiscard]] Image FlipY() const {
            const auto      vec = data.Vector<u8>();
            std::vector<u8> newVec;

            for (i32 j = size.y - 1; j >= 0; j--) {
                for (usize i = 0; i < size.x; i++) {
                    for (auto &c: PixelAt(i, j)) {
                        newVec.push_back(c);
                    }
                }
            }

            return Image{.data = newVec, .size = size, .format = format};
        }
    };
}

#endif //FLK_IMAGE_HPP
