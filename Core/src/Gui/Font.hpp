#ifndef FLK_FONT_HPP
#define FLK_FONT_HPP

#include "Common.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::Gui {
    struct FLK_API Font {
        std::string    filePath;
        Memory::Buffer buffer;
    };
}

#endif //FLK_FONT_HPP
