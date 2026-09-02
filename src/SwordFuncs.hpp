/*
 * Copyright 2020 Tim Hawes <tim@selfdidactic.com>
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

#include <memory>
#include <string>
#include <string_view>
#include <swdisp.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

class SwordFuncs {
private:
  std::unique_ptr<sword::SWMgr> manager;
  sword::SWModule *module = nullptr;
  sword::VerseKey vkey;
  std::string mod_name;
  bool versenum = true;


protected:
  // For derived classes

public:
  SwordFuncs();
  explicit SwordFuncs(std::string);
  virtual ~SwordFuncs() = default;
  bool setModule(std::string_view);
  [[nodiscard]] std::string listBibleVersions() const;

  /**
   * Turn on/off versification for output.
   * @params on Boolean that turns on/off versification for output.
   */
  void versification(bool on);

  [[nodiscard]] bool validModule() const noexcept;

  /* Return current Scripture reference. */
  [[nodiscard]] std::string currentRef() const;

  /* Return current Scripture Text. */
  [[nodiscard]] std::string currentText();

  /* Return the current module name */
  [[nodiscard]] const std::string &modname() const noexcept;

  /* Parse input:
   *  - Change active module
   *  - Lookup verse reference
   */
  std::string parseInput(std::string_view input);

  /* Look up Scripture reference. */
  std::string lookup(std::string_view);

  /* Write an entry in commentary. Module has to be set to a writable module
   * (such as the Sword Personal commentary module).
   */
  bool makeEntry(std::string_view ref, std::string_view input);

  /* Clear a commentary note from a reference.
   */
  bool clearEntry(std::string_view ref);
};

#endif // SWORDFUNCS_HPP
