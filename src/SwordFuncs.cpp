/*
 * SwordFuncs.cpp
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>

#include "SwordFuncs.hpp"

using namespace::sword;

SwordFuncs::SwordFuncs()
{

}

SwordFuncs::SwordFuncs(std::string mod_name)
{
  module = mod_name;
  SwordFuncs();
}

SwordFuncs::~SwordFuncs()
{
}

std::string SwordFuncs::listModules()
{
    ModMap::iterator it;
    std::ostringstream ss;
    for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
      ss << "[" << (*it).second->Name() << "]\t - " << (*it).second->Description() << std::endl;
    }
    return ss.str();
}

std::string SwordFuncs::lookup(std::string ref)
{
  SWMgr manager(new MarkupFilterMgr(FMT_PLAIN));
  SWModule *target = manager.getModule(module.c_str());
  std::string output;

  if (!target) {
    output = listModules();
  } else {
    target->setKey(ref.c_str());
    output = target->RenderText();
  }
  return output;
}
