#include "Output.hpp"

std::string wrapText(std::string_view text, const std::size_t width) {
  if (width == 0) return std::string{text};
  std::string result;
  result.reserve(text.size());
  std::size_t column = 0;
  std::size_t line_start = 0;
  std::size_t last_space = std::string::npos;
  for (const char character : text) {
    result.push_back(character);
    if (character == '\n') {
      column = 0; line_start = result.size(); last_space = std::string::npos;
      continue;
    }
    if (character == ' ') last_space = result.size() - 1;
    ++column;
    if (column < width) continue;
    if (last_space != std::string::npos && last_space >= line_start) {
      result[last_space] = '\n';
      column = result.size() - last_space - 1;
      line_start = last_space + 1;
      last_space = std::string::npos;
    } else {
      result.push_back('\n'); column = 0; line_start = result.size();
    }
  }
  return result;
}
