#include "Options.hpp"
#include "thirdparty/INIReader.h"

#include <cstdlib>
#include <string_view>
#include <utility>

namespace {
constexpr std::string_view config_file = ".cbible.cfg";
constexpr std::string_view default_version = "KJV";

std::string usage() {
  return "Usage: cbible <options>\n\nOptions:\n"
         "  -h [ --help ]              Produce help message\n"
         "  -v [ --version ]           Print version string\n"
         "  -c [ --config ] <path>     Path for a configuration file\n"
         "  -n [ --versenumbers ]      Show output with verse numbers\n"
         "  -i [ --input ]             Send stdin to commentary (requires -r)\n"
         "  -e [ --empty ]             Clear commentary for reference (requires -r)\n"
         "  -b [ --bibleversion ] <id> SWORD module name\n"
         "  -r [ --reference ] <ref>   Scripture reference to look up\n";
}
} // namespace

Options::Options(int argc, char *argv[]) {
  const char *home = std::getenv("HOME");
  opts["default_configfile"] = home != nullptr && *home != '\0'
                                   ? std::string{home} + "/" + std::string{config_file}
                                   : std::string{config_file};
  opts["config"] = opts["default_configfile"];

  bool bible_version_set = false;
  auto fail = [this](std::string message) {
    valid_ = false;
    error_ = std::move(message);
  };
  auto value = [&](int &index, std::string_view option) -> std::string {
    if (index + 1 >= argc || argv[index + 1][0] == '\0') {
      fail("Option '" + std::string{option} + "' requires a non-empty argument");
      return {};
    }
    return argv[++index];
  };

  for (int i = 1; i < argc && valid_; ++i) {
    const std::string_view arg{argv[i]};
    if (arg == "-h" || arg == "--help") opts["help"] = usage();
    else if (arg == "-v" || arg == "--version") opts["version"] = "version";
    else if (arg == "-n" || arg == "--versenumbers") opts["versenumbers"] = "versenumbers";
    else if (arg == "-i" || arg == "--input") opts["input"] = "input";
    else if (arg == "-e" || arg == "--empty") opts["empty"] = "empty";
    else if (arg == "-c" || arg == "--config") opts["config"] = value(i, arg);
    else if (arg.starts_with("--config=")) {
      opts["config"] = std::string{arg.substr(9)};
      if (opts["config"].empty()) fail("Option '--config' requires a non-empty argument");
    }
    else if (arg == "-b" || arg == "--bibleversion") {
      opts["bibleversion"] = value(i, arg);
      bible_version_set = valid_;
    } else if (arg.starts_with("--bibleversion=")) {
      opts["bibleversion"] = std::string{arg.substr(15)};
      bible_version_set = !opts["bibleversion"].empty();
      if (!bible_version_set) fail("Option '--bibleversion' requires a non-empty argument");
    } else if (arg == "-r" || arg == "--reference") opts["reference"] = value(i, arg);
    else if (arg.starts_with("--reference=")) {
      opts["reference"] = std::string{arg.substr(12)};
      if (opts["reference"].empty()) fail("Option '--reference' requires a non-empty argument");
    }
    else fail("Unknown option '" + std::string{arg} + "'");
  }

  if (!valid_ || !opts["help"].empty() || !opts["version"].empty()) return;
  if (!bible_version_set) readIni();
  if (opts["bibleversion"].empty()) opts["bibleversion"] = default_version;
  if (!opts["input"].empty() && opts["reference"].empty()) fail("--input requires --reference");
  else if (!opts["empty"].empty() && opts["reference"].empty()) fail("--empty requires --reference");
  else if (!opts["input"].empty() && !opts["empty"].empty()) fail("--input and --empty cannot be used together");
}

void Options::readIni() {
  INIReader reader(opts["config"]);
  if (reader.ParseError() >= 0) {
    opts["bibleversion"] = reader.Get("", "bibleversion", std::string{default_version});
  }
}

std::string Options::getOption(const std::string &option) const {
  const auto found = opts.find(option);
  return found == opts.end() ? std::string{} : found->second;
}
