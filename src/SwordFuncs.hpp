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

#ifndef SWORDFUNCS_HPP
#define SWORDFUNCS_HPP

#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>
#include <listkey.h>
#include <versekey.h>
#include <swdisp.h>
#include <string>

class SwordFuncs {
 private:
  sword::SWMgr *manager;
  sword::SWModule *module;
  sword::VerseKey vkey;
  std::string mod_name;
  bool versenum = true;

  std::string listModules();

  /* Initialize module for use. Called by all constructors. */
  void SetModule(std::string);


 protected:
  // For derived classes

 public:
  SwordFuncs();
  explicit SwordFuncs(std::string);
  virtual ~SwordFuncs();

  /**
   * Turn on/off versification for output.
   * @params on Boolean that turns on/off versification for output.
   */
  void versification(bool on);

  /* Return current Scripture reference. */
  std::string currentRef();

  /* Return current Scripture Text. */
  std::string currentText();

  /* Return the current module name */
  std::string modname();

  /* Parse input:
   *  - Change active module
   *  - Lookup verse reference
   */
  std::string parseInput(char * input);

  /* Look up Scripture reference. */
  std::string lookup(std::string);

  /* Write an entry in commentary. Module has to be set to a writable module
   * (such as the Sword Personal commentary module).
   */
  bool makeEntry(std::string ref, std::string input);
};

#endif // SWORDFUNCS_HPP
