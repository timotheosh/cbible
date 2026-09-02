#ifndef CBIBLE_OUTPUT_HPP
#define CBIBLE_OUTPUT_HPP
#include <cstddef>
#include <string>
#include <string_view>
[[nodiscard]] std::string wrapText(std::string_view text, std::size_t width);
#endif
