#include "Image.hpp"

#include <stbi/stbi.h>

namespace Pixf::Core::Files {
    Graphics::ImageData LoadImage(const std::string &path, const bool flipVertically) {
        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (data == nullptr) {
            throw ImageLoadError(path);
        }

        return Graphics::ImageData(width, height, channels, data);
    }
}
