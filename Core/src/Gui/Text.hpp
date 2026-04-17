#ifndef FLK_TEXT_HPP
#define FLK_TEXT_HPP

#include "Common.hpp"
#include "Math/Color.hpp"
#include "Serial/Archive.hpp"
#include "Asset/AssetLoader.hpp"

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
        std::string              content;
        u32                      fontSize = 20;
        Asset::AssetHandle<Font> font     = {};
        Color4u8                 color    = Color4u8::White();
        AlignmentH               horizontalAlignment;
        AlignmentV               verticalAlignment;
    };

    FLK_ARCHIVE(Text, content, fontSize, font, color, horizontalAlignment, verticalAlignment)
}

#endif //FLK_TEXT_HPP
