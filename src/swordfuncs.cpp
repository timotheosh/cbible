/*
 * swordfuncs.cpp
 */

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "swordfuncs.hpp"

using namespace::sword;

SwordFuncs::SwordFuncs()
{
}

SwordFuncs::SwordFuncs(char *mod_name)
{
  module = mod_name;
}

SwordFuncs::~SwordFuncs()
{
}

SwordFuncs::listModules()
{
    ModMap::iterator it;
    for (it = library.Modules.begin(); it != library.Modules.end(); it++) {
      fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
    }
}

void SwordFuncs::lookup(char * ref)
{
  target = library.getModule(SwordFuncs::module);
  if (!target) {
    fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
    listModules();
  }

  target->setKey(ref);

  target->RenderText();    // force an entry lookup first to resolve key to something pretty for printing below.

  std::cout << target->getKeyText() << ":\n";
  std::cout << target->RenderText();
  std::cout << "\n";
  std::cout << "==========================\n";
  std::cout << std::endl;
  return 0;
}
