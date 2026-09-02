#include "Options.hpp"
#include "Output.hpp"
#include "SwordFuncs.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

#ifndef CBIBLE_VERSION
#define CBIBLE_VERSION "unknown"
#endif

namespace {
std::size_t terminalWidth() {
  if (isatty(STDOUT_FILENO) == 0) return 0;
  winsize dimensions{};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions) != 0) return 0;
  return dimensions.ws_col;
}

void outputText(const std::string &text) {
  std::cout << wrapText(text, terminalWidth()) << '\n';
}

int interactive(SwordFuncs &sword) {
  rl_bind_key('\t', rl_abort);
  outputText(sword.parseInput("Gen 1:1"));
  using Line = std::unique_ptr<char, decltype(&std::free)>;
  while (true) {
    const std::string prompt = "bible(" + sword.modname() + ") [" + sword.currentRef() + "]> ";
    Line line{readline(prompt.c_str()), &std::free};
    if (!line) break;
    const std::string command{line.get()};
    if (command == "quit" || command == "q") break;
    try {
      outputText(sword.parseInput(command));
      if (!command.empty()) add_history(line.get());
    } catch (const std::exception &error) {
      std::cerr << "cbible: " << error.what() << '\n';
    }
  }
  return 0;
}
} // namespace

int main(int argc, char *argv[]) {
  const Options options{argc, argv};
  if (!options.valid()) {
    std::cerr << "cbible: " << options.error() << '\n';
    return 2;
  }
  if (const std::string help = options.getOption("help"); !help.empty()) {
    std::cout << help;
    return 0;
  }
  if (!options.getOption("version").empty()) {
    std::cout << "cbible Version " << CBIBLE_VERSION << '\n';
    return 0;
  }

  try {
    SwordFuncs sword;
    if (options.getOption("bibleversion").empty()) {
      std::cout << sword.listBibleVersions();
      return 0;
    }
    if (!sword.setModule(options.getOption("bibleversion"))) {
      std::cerr << "cbible: Unknown SWORD module '" << options.getOption("bibleversion") << "'\n";
      std::cerr << sword.listBibleVersions();
      return 3;
    }
    const std::string reference = options.getOption("reference");
    if (reference.empty()) return interactive(sword);
    if (!options.getOption("versenumbers").empty()) sword.versification(true);
    else sword.versification(false);

    if (!options.getOption("empty").empty() && !sword.clearEntry(reference)) {
      std::cerr << "cbible: Module '" << sword.modname() << "' is not writable\n";
      return 4;
    }
    if (!options.getOption("input").empty()) {
      std::ostringstream input;
      input << std::cin.rdbuf();
      if (!sword.makeEntry(reference, input.str())) {
        std::cerr << "cbible: Module '" << sword.modname() << "' is not writable\n";
        return 4;
      }
    } else if (options.getOption("empty").empty()) {
      outputText(sword.lookup(reference));
    }
  } catch (const std::invalid_argument &error) {
    std::cerr << "cbible: " << error.what() << '\n';
    return 2;
  } catch (const std::exception &error) {
    std::cerr << "cbible: " << error.what() << '\n';
    return 1;
  }
  return 0;
}
