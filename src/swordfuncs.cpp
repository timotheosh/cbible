/*
 * swordfuncs.cpp
 */

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "swordfuncs.hpp"

using namespace::sword;

swordfuncs::swordfuncs()
{
  swordfuncs::library = SWMgr(new MarkupFilterMgr(FMT_PLAIN));
}

swordfuncs::swordfuncs(char *mod_name)
{
  swordfuncs::module = mod_name;
  swordfuncs();
}

swordfuncs::~swordfuncs()
{
}

void swordfuncs::listModules()
{
    ModMap::iterator it;
    for (it = swordfuncs::library.Modules.begin(); it != swordfuncs::library.Modules.end(); it++) {
      fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
    }
}

void swordfuncs::lookup(char * ref)
{
  target = library.getModule(swordfuncs::module);
  if (!target) {
    fprintf(stderr, "Could not find module [%s].  Available modules:\n", swordfuncs::module);
    swordfuncs::listModules();
  }

  target->setKey(ref);

  target->RenderText();    // force an entry lookup first to resolve key to something pretty for printing below.

  std::cout << target->getKeyText() << ":\n";
  std::cout << target->RenderText();
  std::cout << "\n";
  std::cout << "==========================\n";
  std::cout << std::endl;
}
