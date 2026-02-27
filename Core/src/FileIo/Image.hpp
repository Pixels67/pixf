#ifndef FLK_FILEIO_IMAGE_HPP
#define FLK_FILEIO_IMAGE_HPP

#include <filesystem>

#include "Common.hpp"
#include "Graphics/Image.hpp"

namespace Flock::FileIo {
    Graphics::Image FLK_API ReadImage(const std::filesystem::path& filePath);
}

#endif //FLK_FILEIO_IMAGE_HPP