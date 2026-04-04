#ifndef FLK_GUI_IMAGE_HPP
#define FLK_GUI_IMAGE_HPP

#include "Common.hpp"
#include "Reflect.hpp"

namespace Flock::Gui {
    struct FLK_API Image {
        std::string imagePath;
    };

    inline auto Reflect(Image &image) {
        return Reflectable{
            "Image",
            std::make_tuple(
                Field{"image", &image.imagePath}
            )
        };
    }
}

#endif //FLK_GUI_IMAGE_HPP
