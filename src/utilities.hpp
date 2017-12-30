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

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>

/**
 * Code for trimming whitespace from strings.
 * Copied from
 * http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */

// trim from start
static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(),
          std::find_if(s.begin(), s.end(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace)))
              .base(),
          s.end());
  return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) { return ltrim(rtrim(s)); }

#endif // UTILITIES_HPP
