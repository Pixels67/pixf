#ifndef FLK_BUTTON_HPP
#define FLK_BUTTON_HPP

#include "Common.hpp"
#include "Math/Color.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Gui {
    struct FLK_API Button {
        std::string imagePath;
        Color4u8    defaultColor = Color4u8::Black();
        Color4u8    hoverTint    = Color4u8{0, 0, 0, 50};
        Color4u8    pressTint    = Color4u8{0, 0, 0, 20};
        std::string onPressEvent;
        std::string onReleaseEvent;
    };

    FLK_ARCHIVE(Button, imagePath, defaultColor, hoverTint, pressTint, onPressEvent, onReleaseEvent)
}

#endif //FLK_BUTTON_HPP
