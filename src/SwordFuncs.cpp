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

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>

#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>
#include <listkey.h>
#include <versekey.h>
#include <swdisp.h>

#include "utilities.hpp"
#include "SwordFuncs.hpp"

using namespace sword;

SwordFuncs::SwordFuncs()
{
  if (mod_name.empty())
    mod_name = "KJV";
  SetModule(mod_name);
}

SwordFuncs::SwordFuncs(std::string module_name)
{
  mod_name = module_name;
  SwordFuncs();
}

SwordFuncs::~SwordFuncs()
{
}

void SwordFuncs::SetModule(std::string module_name)
{
  manager = new SWMgr(new MarkupFilterMgr(FMT_PLAIN));
  module = manager->getModule(module_name.c_str());

  if (!module) {
    std::cout << listModules() << std::endl;
  } else {
    mod_name = module_name;
  }
}

std::string SwordFuncs::parseInput(char * input)
{
  std::string str = input;
  if (str.compare(0,1,"?") == 0)
  {
    std::string mod = str.substr(1);
    trim(mod);
    SetModule(mod);
    return(mod);
  }
  else
    return lookup(str);
}

std::string SwordFuncs::listModules()
{
  ModMap::iterator it;
  std::ostringstream ss;
  for (it = manager->Modules.begin(); it != manager->Modules.end(); it++) {
    ss << "[" << (*it).second->Name() << "]\t - " << (*it).second->Description() << std::endl;
  }
  return ss.str();
}

std::string SwordFuncs::modname()
{
  return mod_name;
}

std::string SwordFuncs::lookup(std::string ref)
{
  std::string output = "";
  //Module variables
  sword::VerseKey vk;

  //Variables related to splitting up the reference for iteration
  sword::ListKey refRange = vk.ParseVerseList(ref.c_str(), vk, true);
  refRange.Persist(true);
  module->setKey(refRange);

  int i = 0;
  try
  {
    for((*module) = sword::TOP; !module->Error(); (*module)++) {
      sword::VerseKey nk(module->getKey());
      output += " ";
      output += std::to_string(nk.getVerse());
      output += " ";
      output += module->RenderText();
    }
    output += "\n";
    output += module->getKey()->getRangeText();
  }
  catch(const std::runtime_error& re)
  {
    // speciffic handling for runtime_error
    std::cerr << "Runtime error: " << re.what() << std::endl;
  }
  catch(const std::exception& ex)
  {
    // speciffic handling for all exceptions extending std::exception, except
    // std::runtime_error which is handled explicitly
    std::cerr << "Error occurred: " << ex.what() << std::endl;
  }
  catch(...)
  {
    // catch any other errors (that we have no information about)
    std::cerr << "Unknown failure occured. Possible memory corruption" << std::endl;
  }
  return output;
}
