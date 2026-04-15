#ifndef BOX_HPP
#define BOX_HPP

#include "Common.hpp"
#include "Math/Color.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Gui {
    struct FLK_API Box {
        Color4u8 color = Color4u8::Black();
    };

    FLK_ARCHIVE(Box, color)
}

#endif //BOX_HPP
