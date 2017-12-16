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

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include "SwordFuncs.hpp"
#include "Options.hpp"

#define CBIBLE_VERSION "0.20"

void OutputText(std::string s);

int main(int argc, char *argv[]) {
  Options options(argc, argv);
  std::string help = options.getOption("help");
  std::string bibleversion = options.getOption("bibleversion");
  std::string reference = options.getOption("reference");
  std::string versenumbers = options.getOption("versenumbers");
  std::string inputtext = options.getOption("input");

  /* Display usage and exit */
  if (!help.empty()) {
    std::cout << help << std::endl;
    return(0);
  } else if (!options.getOption("version").empty()) {
    std::cout << "cbible Version " << CBIBLE_VERSION << std::endl;
    return(0);
  }
  if (bibleversion.empty())
    bibleversion = "KJV";

  SwordFuncs *sw = new SwordFuncs(bibleversion);

  /* Use interactive mode */
  if (reference.empty()) {
    char *buf;
    rl_bind_key('\t', rl_abort);  // disable auto-complete

    try {
      OutputText("sw->parseInput(const_cast<char *>(\"Gen 1:1\"))");
      OutputText(sw->parseInput(const_cast<char *>("Gen 1:1")));
      while ((buf = readline(("bible(" + sw->modname() + ") ["
                              + sw->currentRef() + "]> ").c_str())) != NULL) {
        if ((strcmp(buf, "quit") == 0) ||
            (strcmp(buf, "q") == 0))
          break;

        try {
          OutputText(sw->parseInput(buf).c_str());
        } catch (std::exception &e) {
          std::cout << e.what() << std::endl;
        }

        if (buf[0] != 0)
          add_history(buf);
      }
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
    free(buf);
  } else {
    /* Lookup the reference and exit. */
    if (versenumbers.empty()) {
      sw->versification(false);
    }
    if (inputtext.empty()) {
      OutputText(sw->parseInput(const_cast<char *>(reference.c_str())));
    } else {
      std::string line;
      std::stringstream is;
      while ( std::getline(std::cin, line) ) {
        is << line << std::endl;
      }
      sw->makeEntry(reference, is.str());
    }
  }
  free(sw);
  return 0;
}


void OutputText(std::string s) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int bufferWidth = w.ws_col;

  /* If executed from Emacs' eshell, bufferWidth will be 0. */
  if (bufferWidth > 0) {
    for (unsigned int i = 1; i <= s.length() ; i++) {
      char c = s[i-1];

      int spaceCount = 0;

      // Add whitespace if newline detected.
      if (c == '\n') {
        int charNumOnLine = ((i) % bufferWidth);
        spaceCount = bufferWidth - charNumOnLine;
        /* insert space before newline break */
        s.insert((i-1), (spaceCount), ' ');
        /* jump forward in string to character at beginning of next line. */
        i+=(spaceCount);
        continue;
      }

      if ((i % bufferWidth) == 0) {
        if (c != ' ') {
          for (int j = (i-1); j > -1 ; j--) {
            if (s[j] == ' ') {
              s.insert(j, spaceCount, ' ');
              break;
            } else {
              spaceCount++;
            }
          }
        }
      }
    }
  }
  // Output string to console
  std::cout << s << std::endl;
}
