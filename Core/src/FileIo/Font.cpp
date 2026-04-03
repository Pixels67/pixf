#include "Font.hpp"

#include "File.hpp"

namespace Flock::FileIo {
    std::optional<Gui::Font> ReadFont(const std::filesystem::path &filePath) {
        const auto text = ReadText(filePath);
        if (!text) {
            return std::nullopt;
        }

        Gui::Font font;
        font.filePath = filePath.string();
        font.buffer   = Memory::Buffer(text->data(), text->size());

        return font;
    }
}
