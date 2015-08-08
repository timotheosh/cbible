/*
 * Copyright 2015 Tim Hawes <tim@selfdidactic.com>
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

#include <boost/program_options.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include "Options.hpp"


Options::Options(int argc, char *argv[]) {
  try {
    int opt;
    std::string home;
    char *p_home = getenv("HOME");
    if (p_home != NULL)
      home = p_home;
    std::string default_config = home + "/.cbible.cfg";
    std::string config_file;
    std::stringstream s_help_options;

    // Variables for options
    std::string bibleversion;
    std::string reference;

    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help", "produce help message")
        ("config,c",
         po::value<std::string>(&config_file)->
         default_value(default_config.c_str()),
         "name of a configuration file.")
        ("versenumbers,n", "turn on verse numbers")
        ("input,i", "Send stdin to specified commentary reference.");

    po::options_description config("Configuration");
    config.add_options()
        ("bibleversion,b",
         po::value<std::string>(&bibleversion)->default_value("KJV"),
         "Bible version (Using Sword's 3 letter acronym).")
        ("reference,r",
         po::value<std::string>(&reference)->default_value(""),
         "Scripture reference to lookup");

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config);

    po::options_description config_file_options;
    config_file_options.add(config);
    po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).run(), varmap);
    po::notify(varmap);

    if (varmap.count("help")) {
      std::ostringstream s_help_options;
      s_help_options << cmdline_options;
      help_options = s_help_options.str();
    }

    std::ifstream ifs(config_file.c_str());
    if (!ifs) {
      createConfig(config_file);
    } else {
      po::store(parse_config_file(ifs, config_file_options), varmap);
      po::notify(varmap);
    }
  } catch (std::exception &e) {
    std::cerr << "Exception caught in Options object: "
              << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception caught in Options object."
              << std::endl;
  }
}

Options::~Options() {
}

void Options::createConfig(std::string configfile) {
  std::ofstream ofs;
  try {
    ofs.open(configfile.c_str());
    if (ofs) {
      ofs << "bibleversion = " << varmap["bibleversion"].as<std::string>()
          << std::endl;
    } else {
      throw std::invalid_argument(
          "Cannot open non-existent config file for writing.");
    }
  } catch (std::exception &e) {
    std::cerr << "Exception thrown during config file creation: "
              << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception thrown during config file creation."
              << std::endl;
  }
  ofs.close();
}

std::string Options::getOption(std::string option) {
  std::ostringstream ret;
  if (varmap.count(option.c_str())) {
    if (option == "help") {
      ret << help_options;
    } else if (option == "version") {
      ret << "true";
    } else if (option == "versenumbers") {
      ret << "true";
    } else if (option == "input") {
      ret << "true";
    } else {
      ret << varmap[option].as<std::string>();
    }
  }
  return ret.str();
}

std::string Options::getOption(const char *opt) {
  std::string option = opt;
  return getOption(option);
}
