#ifndef FLK_BUTTON_HPP
#define FLK_BUTTON_HPP

#include "Common.hpp"
#include "Math/Color.hpp"

namespace Flock::Gui {
    struct FLK_API Button {
        std::string imagePath;
        Color4u8    defaultColor  = Color4u8::Black();
        Color4u8    hoverTint = Color4u8{0, 0, 0, 50};
        Color4u8    pressTint     = Color4u8{0, 0, 0, 20};
        std::string onPressEvent;
        std::string onReleaseEvent;
    };

    inline auto Reflect(Button &button) {
        return Reflectable{
            "Button",
            std::make_tuple(
                Field{"image", &button.imagePath},
                Field{"defaultColor", &button.defaultColor},
                Field{"hoverTint", &button.hoverTint},
                Field{"pressTint", &button.pressTint},
                Field{"onPressEvent", &button.onPressEvent},
                Field{"onReleaseEvent", &button.onReleaseEvent}
            )
        };
    }
}

#endif //FLK_BUTTON_HPP
