#include "Font.hpp"

#include <string>

#include "File.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::FileIo {
    std::optional<Gui::Font> ReadFont(const std::filesystem::path &filePath) {
        const auto bytes = ReadBytes(filePath);
        if (!bytes) {
            return std::nullopt;
        }

        Gui::Font font;
        font.filePath = filePath.string();
        font.buffer   = Memory::Buffer(bytes->data(), bytes->size());

        return font;
    }
}
