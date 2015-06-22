/*
 * Copyright 2015 <copyright holder> <email>
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

#include "SwordFuncs.hpp"

using namespace::sword;

SwordFuncs::SwordFuncs()
{
  if (mod_name.empty())
    mod_name = "KJV";
  manager = new SWMgr(new MarkupFilterMgr(FMT_PLAIN));
  module = manager->getModule(mod_name.c_str());

  if (!module) {
    std::cout << listModules() << std::endl;
  }

}

SwordFuncs::SwordFuncs(std::string module_name)
{
  mod_name = module_name;
  SwordFuncs();
}

SwordFuncs::~SwordFuncs()
{
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
  for((*module) = sword::TOP; !module->Error(); (*module)++) {
    sword::VerseKey nk(module->getKey());
    output += " ";
    output += std::to_string(nk.getVerse());
    output += " ";
    output += module->RenderText();    
  }
  output += "\n";
  output += module->getKey()->getRangeText();
  return output;
}
