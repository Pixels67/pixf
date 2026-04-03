#ifndef FLK_FILEIO_FONT_HPP
#define FLK_FILEIO_FONT_HPP

#include <filesystem>

#include "Gui/Font.hpp"

namespace Flock::FileIo {
    std::optional<Gui::Font> ReadFont(const std::filesystem::path &filePath);
}

#endif //FLK_FILEIO_FONT_HPP