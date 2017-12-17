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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "thirdparty/optionparser.h"
#include "thirdparty/INIReader.h"
#include "Options.hpp"

#define PROGNAME "cbible"

struct Arg: public option::Arg {
  static void printError(const char* msg1, const option::Option& opt,
                         const char* msg2) {
    fprintf(stderr, "%s", msg1);
    fwrite(opt.name, opt.namelen, 1, stderr);
    fprintf(stderr, "%s", msg2);
  }


  static option::ArgStatus Unknown(const option::Option& option, bool msg) {
    if (msg) printError("Unknown option '", option, "'\n");
    return option::ARG_ILLEGAL;
  }


  static option::ArgStatus NonEmpty(const option::Option& option, bool msg) {
  /* These option keys will always exist, we'll only need to check them. */

    if (option.arg != 0 && option.arg[0] != 0)
      return option::ARG_OK;
    if (msg) {
      if ((strcmp(option.name, "c") == 0) ||
          (strcmp(option.name, "--config") == 0))
        printError("Option '", option, "' requires a path to a config file.\n");
      else
        printError("Option '", option, "' requires a non-empty argument\n");
    }
    return option::ARG_ILLEGAL;
  }
};

enum optionIndex { UNKNOWN, HELP, VERSION, CONFIG, VERSENUMBERS, INPUT,
                   BIBLEVERSION, REFERENCE};

const option::Descriptor usage[] = {
  {UNKNOWN, 0, "", "", option::Arg::None,
   "Usage: " PROGNAME " <options>\n\n" "Options:" },
  {HELP, 0, "h", "help", option::Arg::None,
   "  -h [ --help ]  \tProduce help message"},
  {VERSION, 0, "v", "version", option::Arg::None,
   "  -v [ --version ]  \tPrint version string"},
  {CONFIG, 0, "c", "config", Arg::NonEmpty,
   "  -c [ --config ]  <path>  \tPath for a configuration file"},
  {VERSENUMBERS, 0, "n", "versenumbers", option::Arg::None,
   "  -n [ --versenumbers ]  \tShow output with verse numbers"},
  {INPUT, 0, "i", "input", option::Arg::None,
   "Send stdin to personal commentary (must be used with -r)."},
  {BIBLEVERSION, 0, "b", "bibleversion", Arg::NonEmpty,
   "  -b [ --bibleversion ] <bible version>  \tBible version "
   "(using Sword's 3 letter acronym)"},
  {REFERENCE, 0, "r", "reference", Arg::NonEmpty,
   "  -r [ --reference ] <reference>  \tScripture reference to look up"},
  {0, 0, 0, 0, 0, 0}
};



Options::Options(int argc, char *argv[]) {
  argc -= (argc > 0); argv += (argc > 0);  // skip program name argv[0]
  option::Stats  stats(usage, argc, argv);
  std::vector<option::Option> options(stats.options_max);
  std::vector<option::Option> buffer(stats.buffer_max);
  option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

  if (options[HELP]) {
    std::stringstream hs;
    option::printUsage(hs, usage);
    Options::opts["help"] = hs.str();
  } else if (options[VERSION]) {
    Options::opts["version"] = "version";
  } else {
    /* set up value for default config file. */
    std::string s_home = getenv("HOME");
    if (!s_home.empty())
      Options::opts["default_configfile"] = s_home + "/" + CONFIGFILE;
    else
      Options::opts["default_configfile"] = CONFIGFILE;

    /* If no config file was specified, use the default. */
    if (options[CONFIG]) {
      Options::opts["config"] = options[CONFIG].arg;
    } else {
      Options::opts["config"] = Options::opts["default_configfile"];
    }

    if (options[BIBLEVERSION]) {
      Options::opts["bibleversion"] = options[BIBLEVERSION].arg;
    } else {
      readIni();
    }

    if (options[REFERENCE]) {
      Options::opts["reference"] = options[REFERENCE].arg;
    }

    Options::opts["input"] = "";
    if (options[INPUT]) {
      if (!options[REFERENCE]) {
        std::cerr << "-i input option cannot be used without a -r reference "
                  << "(\"You want me to put this... where?\")" <<std::endl;
      } else {
        Options::opts["input"] = "input";
      }
    }
  }  // end else
}

Options::~Options() {
}

void Options::readIni() {
  /**
   * Reads settings from ini file.
   */
  INIReader reader(Options::opts["config"]);
  if (reader.ParseError() < 0) {
    std::cerr << "Cannot read configuration from " << Options::opts["config"]
              << std::endl;
    return;
  }
  if (Options::opts["bibleversion"].empty())
    Options::opts["bibleversion"] =
        reader.Get("", "bibleversion", DEFAULT_VERSION);
}

void Options::checkConfig() {
  /**
   * Checks for the existence of the default config file. If it does
   * not exist, it will create one.
   */
  FILE *cfgfile = fopen(Options::opts["default_configfile"].c_str(), "r");
  if (cfgfile == NULL)
    createConfig();
  else
    fclose(cfgfile);
}

void Options::createConfig() {
  /**
   * Creates a new config file.
   *
   * @params configfile Path for the new config file.
   */
  if (Options::opts["bibleversion"].empty())
    Options::opts["bibleversion"] = DEFAULT_VERSION;
  std::ofstream ofs;
  try {
    ofs.open(Options::opts["default_configfile"].c_str());
    if (ofs) {
      ofs << "bibleversion = " << Options::opts["bibleversion"] << std::endl;
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

std::string Options::getOption(std::string opt) {
  return Options::opts[opt];
}

std::string Options::getOption(const char *opt) {
  std::string s = opt;
  return Options::getOption(s);
}
