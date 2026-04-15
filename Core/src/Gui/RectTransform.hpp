#ifndef FLK_RECTTRANSFORM_HPP
#define FLK_RECTTRANSFORM_HPP

#include "Common.hpp"
#include "Math/Rect.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Gui {
    struct FLK_API RectTransform {
        Rect2i rect;
    };

    FLK_ARCHIVE(RectTransform, rect)
}

#endif //FLK_RECTTRANSFORM_HPP
