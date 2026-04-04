#ifndef FLK_BUTTON_HPP
#define FLK_BUTTON_HPP

#include "Common.hpp"
#include "Math/Color.hpp"

namespace Flock::Gui {
    struct FLK_API Button {
        std::string           imagePath;
        Color4u8              defaultColor   = Color4u8::Black();
        Color4u8              highlightColor = Color4u8{50, 50, 50, 255};
        Color4u8              pressColor     = Color4u8{20, 20, 20, 255};
        std::function<void()> onPress;
        std::function<void()> onRelease;
    };

    inline auto Reflect(Button &button) {
        return Reflectable{
            "Button",
            std::make_tuple(
                Field{"image", &button.imagePath},
                Field{"defaultColor", &button.defaultColor},
                Field{"highlightColor", &button.highlightColor},
                Field{"pressColor", &button.pressColor}
            )
        };
    }
}

#endif //FLK_BUTTON_HPP
