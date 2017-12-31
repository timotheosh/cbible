/*
 * Copyright 2016 Tim Hawes <tim@selfdidactic.com>
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

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sword/listkey.h>
#include <sword/markupfiltmgr.h>
#include <sword/swdisp.h>
#include <sword/swmgr.h>
#include <sword/swmodule.h>
#include <sword/versekey.h>

#include "SwordFuncs.hpp"
#include "utilities.hpp"

SwordFuncs::SwordFuncs() {
  if (mod_name.empty())
    mod_name = "KJV";
  SetModule(mod_name);
}

SwordFuncs::SwordFuncs(std::string module_name) {
  mod_name = module_name;
  SetModule(mod_name);
}

SwordFuncs::~SwordFuncs() {
  free(module);
  free(manager);
}

void SwordFuncs::SetModule(std::string module_name) {
  manager = new sword::SWMgr(new sword::MarkupFilterMgr(sword::FMT_PLAIN));
  module = manager->getModule(module_name.c_str());

  if (!module) {
    module = manager->getModule(mod_name.c_str());
    std::cout << listModules() << std::endl;
  } else {
    module->setKey(vkey);
    mod_name = module_name;
  }
}

void SwordFuncs::versification(bool on) { versenum = on; }

std::string SwordFuncs::currentRef() {
  std::string ret = vkey.getText();
  if (ret.empty())
    ret = "foo";
  return ret;
}

std::string SwordFuncs::currentText() {
  module->setKey(vkey);
  std::ostringstream os;

  sword::SWBuf buffer = module->renderText();
  std::string text = buffer.c_str();
  if (versenum) {
    os << " " << vkey.getVerse();
  }
  os << " " << trim(text);
  return os.str();
}

std::string SwordFuncs::parseInput(char *input) {
  std::string str = input;
  if (str.compare(0, 2, "??") == 0) {
    std::cout << "perform global search" << std::endl;
  } else if (str.compare(0, 1, "?") == 0) {
    std::cout << "perform search within module" << std::endl;
  } else if (str.compare(0, 1, "!") == 0) {
    std::string mod = str.substr(1);
    trim(mod);
    SetModule(mod);
  } else if (str.empty()) {
    if (vkey.isTraversable()) {
      vkey++;
    }
  } else {
    return lookup(str);
  }
  return currentText();
}

std::string SwordFuncs::listModules() {
  sword::ModMap::iterator it;
  std::ostringstream ss;
  for (it = manager->Modules.begin(); it != manager->Modules.end(); it++) {
    ss << "[" << (*it).second->getName() << "]\t - "
       << (*it).second->getDescription() << std::endl;
  }
  return ss.str();
}

std::string SwordFuncs::modname() { return mod_name; }

std::string SwordFuncs::lookup(std::string ref) {
  std::ostringstream output;

  // Set up module specific variables
  sword::VerseKey vk;

  // Variables related to splitting up the reference for iteration
  sword::ListKey refRange = vk.parseVerseList(ref.c_str(), vk, true);
  refRange.setPersist(true);
  module->setKey(refRange);

  try {
    int i = 0;
    for ((*module) = sword::TOP; !module->popError(); (*module)++) {
      i++;
      sword::VerseKey nk(module->getKey());
      sword::SWBuf buffer = module->renderText();
      std::string text = buffer.c_str();
      if (versenum) {
        output << " " << nk.getVerse();
      }
      output << " " << trim(text);
    }
    if (i > 1)
      output << std::endl << module->getKey()->getRangeText();
    vkey = module->getKey();
  } catch (const std::runtime_error &re) {
    // speciffic handling for runtime_error
    std::cerr << "Runtime error: " << re.what() << std::endl;
  } catch (const std::exception &ex) {
    // speciffic handling for all exceptions extending std::exception, except
    // std::runtime_error which is handled explicitly
    std::cerr << "Error occurred: " << ex.what() << std::endl;
  } catch (...) {
    // catch any other errors (that we have no information about)
    std::cerr << "Unknown failure occured. Possible memory corruption"
              << std::endl;
  }
  return output.str();
}

bool SwordFuncs::makeEntry(std::string ref, std::string input) {
  bool ret = module->isWritable();
  if (ret) {

    // Set the key according to the reference.
    vkey.setText(const_cast<char *>(ref.c_str()));
    module->setKey(vkey);

    // Check for any exisitng comments
    std::string ctext = currentText(); //trim(s1);
    std::string data;

    if (ctext.empty()) {
      data = input;
    } else {
      data = ctext + "<br/><br/>\n\n" + input;
    }
    const char *entry = const_cast<char *>(data.c_str());
    int64_t length = data.length();
    module->setEntry(entry, length);
  }
  return ret;
}

bool SwordFuncs::clearEntry(std::string ref) {
  bool ret = module->isWritable();
  if (ret) {
    vkey.setText(const_cast<char *>(ref.c_str()));
    module->setKey(vkey);
    module->setEntry("", 0);
  }
  return ret;
}

sword::ListKey SwordFuncs::search(int type, std::string search_terms,
                                  std::string search_scope) {
  sword::SWKey *key;
  return module->search(const_cast<char *>(search_terms.c_str()), type, 0, key);
}
