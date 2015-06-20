
/*
 * swordfuncs.hpp
 */

#ifndef SWORDFUNCS_HPP
#define SWORDFUNCS_HPP

#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

class swordfuncs
{
 private:
  sword::SWMgr library;
  sword::SWModule *target;
  const char *module = "KJV";

  void listModules();

 protected:
  // For derived classes

 public:
  swordfuncs();
  swordfuncs(char *);
  virtual ~swordfuncs();
  void lookup(char *);
};

#endif // SWORDFUNCS_HPP
