/*
 * Copyright 2017 Tim Hawes <tim@selfdidactic.com>
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
 * Class Options
 *
 * Description: Handles options for running the cbible application. It
 * handles both a configuration from an ini file and command line
 * options.
 */

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#define CONFIGFILE ".cbible.cfg"
#define DEFAULT_VERSION "KJV"
#define DEFAULT_REFERENCE "Gen 1:1"

#include <string>
#include <map>

class Options {
 private:
  /* Vars for holding the option data for cbible */
  std::map<std::string, std::string> opts;

  /* Read options from ini file. */
  void readIni();

  /* Creates the default config file. */
  void createConfig();

  /* Checks for existence of default config file. */
  void checkConfig();

 public:
  Options(int argc, char *argv[]);
  ~Options();
  std::string getOption(std::string);
  std::string getOption(const char *);
};

#endif  // OPTIONS_HPP
