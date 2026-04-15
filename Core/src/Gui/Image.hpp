#ifndef FLK_GUI_IMAGE_HPP
#define FLK_GUI_IMAGE_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Gui {
    struct FLK_API Image {
        std::string imagePath;
    };

    FLK_ARCHIVE(Image, imagePath)
}

#endif //FLK_GUI_IMAGE_HPP
