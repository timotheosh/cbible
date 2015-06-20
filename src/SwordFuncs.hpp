/*
 * SwordFuncs.hpp
 */

#ifndef SWORDFUNCS_HPP
#define SWORDFUNCS_HPP

#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

class SwordFuncs
{
 private:
  sword::SWMgr manager;
  sword::SWModule *target;
  std::string module = "KJV";

  std::string listModules();

 protected:
  // For derived classes

 public:
  SwordFuncs();
  SwordFuncs(std::string);
  virtual ~SwordFuncs();
  std::string lookup(std::string);
};

#endif // SWORDFUNCS_HPP
