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

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Options {
 private:
  po::variables_map varmap;
  std::string help_options;
  void createConfig(std::string configfile);

 public:
  Options(int argc, char *argv[]);
  ~Options();
  std::string getOption(std::string);
  std::string getOption(const char *);
};

#endif // OPTIONS_HPP
