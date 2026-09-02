/*
 * Copyright 2016 Tim Hawes <tim@selfdidactic.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// Utility functions.

#ifndef CBIBLE_UTILITIES_HPP
#define CBIBLE_UTILITIES_HPP

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

/**
 * Code for trimming whitespace from strings.
 * Copied from
 * http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */

// trim from start
[[nodiscard]] inline std::string trim(std::string_view value) {
  const auto is_space = [](unsigned char c) { return std::isspace(c) != 0; };
  const auto first = std::find_if_not(value.begin(), value.end(), is_space);
  const auto last = std::find_if_not(value.rbegin(), value.rend(), is_space).base();
  if (first >= last) {
    return {};
  }
  return {first, last};
}

#endif
