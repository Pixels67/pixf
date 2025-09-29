#include "string_utils.h"

#include <string>

namespace pixf::core {
std::pair<std::string, std::string> SliceStrAtDelimiter(
    const std::string_view input, const std::string_view delimiter) {
  const size_t pos = input.find(delimiter);

  if (pos == std::string::npos) {
    return {std::string(input), ""};
  }

  const size_t lineStart = input.rfind('\n', pos) + 1;
  const size_t lineEnd = input.find('\n', pos) + 1;

  std::string beforeDelimiter(input.substr(0, lineStart));
  std::string afterDelimiter(input.substr(lineEnd));

  return {beforeDelimiter, afterDelimiter};
}

std::string ReplaceAll(std::string str, const std::string_view query,
                       const std::string_view value) {
  size_t pos = 0;
  while ((pos = str.find(query, pos)) != std::string::npos) {
    str.replace(pos, query.length(), value);
    pos += value.length();
  }

  return str;
}
}  // namespace pixf::core
