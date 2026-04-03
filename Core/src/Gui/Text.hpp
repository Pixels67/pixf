#ifndef FLK_TEXT_HPP
#define FLK_TEXT_HPP

#include "Common.hpp"
#include "Math/Color.hpp"

namespace Flock::Gui {
    enum AlignmentH {
        Left,
        Center,
        Right,
    };

    enum AlignmentV {
        Top,
        Middle,
        Bottom,
    };

    struct FLK_API Text {
        std::string content;
        u32         fontSize = 20;
        std::string fontPath = "";
        Color4u8    color    = Color4u8::White();
        AlignmentH  horizontalAlignment;
        AlignmentV  verticalAlignment;
    };

    inline auto Reflect(Text &text) {
        return Reflectable{
            "Text",
            std::make_tuple(
                Field{"content", &text.content},
                Field{"fontSize", &text.fontSize},
                Field{"font", &text.fontPath},
                Field{"color", &text.color},
                Field{"horizontalAlignment", &text.horizontalAlignment},
                Field{"verticalAlignment", &text.verticalAlignment}
            )
        };
    }
}

#endif //FLK_TEXT_HPP
