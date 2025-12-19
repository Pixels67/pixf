#ifndef PIXF_RECT_HPP
#define PIXF_RECT_HPP

#include "Math/Vector.hpp"

namespace Pixf::Core::Gui {
    struct Rect {
        Math::Vector2u origin;
        Math::Vector2u aspect;
    };
} // namespace Pixf::Core::Gui

#endif // PIXF_RECT_HPP
