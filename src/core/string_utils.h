#pragma once
#include <string>

namespace Engine::Core {
    std::pair<std::string, std::string> SliceStrAtDelimiter(std::string_view input, std::string_view delimiter);
    std::string ReplaceAll(std::string str, std::string_view query, std::string_view value);
} // namespace Engine::Core
