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

#ifndef SWORDFUNCS_HPP
#define SWORDFUNCS_HPP

#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>
#include <listkey.h>
#include <versekey.h>
#include <swdisp.h>

class SwordFuncs
{
 private:
  sword::SWMgr *manager;
  sword::SWModule *module;
  sword::VerseKey vkey;
  std::string mod_name;

  std::string listModules();

  void SetModule(std::string);

 protected:
  // For derived classes

 public:
  SwordFuncs();
  SwordFuncs(std::string);
  virtual ~SwordFuncs();
  std::string currentRef();
  std::string modname();
  std::string parseInput(char * input);
  std::string lookup(std::string);
};

#endif // SWORDFUNCS_HPP
