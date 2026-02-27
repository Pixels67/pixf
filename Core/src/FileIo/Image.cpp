#include "Image.hpp"

#include <stbi.h>

namespace Flock::FileIo {
    Graphics::Image ReadImage(const std::filesystem::path& filePath) {
        stbi_set_flip_vertically_on_load(1);

        i32   width, height, channels;
        void *data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 4);

        auto format = Graphics::ImageFormat::Rgba;
        switch (channels) {
            case 1:
                format = Graphics::ImageFormat::R;
                break;
            case 2:
                format = Graphics::ImageFormat::Rg;
                break;
            case 3:
                format = Graphics::ImageFormat::Rgb;
                break;
            case 4:
                format = Graphics::ImageFormat::Rgba;
                break;
            default:
                break;
        }

        Graphics::Image image{
            .data   = Memory::Buffer(data, width * height * channels),
            .size   = Vector2i{width, height},
            .format = format
        };

        stbi_image_free(data);

        return image;
    }
}
