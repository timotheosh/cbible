
/*
 * swordfuncs.hpp
 */

#ifndef SWORDFUNCS_HPP
#define SWORDFUNCS_HPP

#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

class SwordFuncs
{
 private:
  sword::SWMgr library(new MarkupFilterMgr(FMT_PLAIN));
  sword::SWModule *target;
  std::string module = 'KJV';

  void listModules();

 protected:
  // For derived classes

 public:
  SwordFuncs();
  SwordFuncs(char *);
  virtual ~SwordFuncs();
  lookup(char *);
}

#endif // SWORDFUNCS_HPP
