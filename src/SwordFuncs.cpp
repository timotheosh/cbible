#include "SwordFuncs.hpp"
#include "utilities.hpp"

#include <cctype>
#include <cstdint>
#include <markupfiltmgr.h>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
class ValidatingVerseKey final : public sword::VerseKey {
public:
  using sword::VerseKey::getBookFromAbbrev;
};
} // namespace

SwordFuncs::SwordFuncs() { setModule("KJV"); }
SwordFuncs::SwordFuncs(std::string module_name) { setModule(module_name); }

bool SwordFuncs::setModule(const std::string_view module_name) {
  auto candidate = std::make_unique<sword::SWMgr>(
      new sword::MarkupFilterMgr(sword::FMT_PLAIN));
  auto *candidate_module = candidate->getModule(std::string{module_name}.c_str());
  if (candidate_module == nullptr) return false;

  candidate_module->setKey(vkey);
  manager = std::move(candidate);
  module = candidate_module;
  mod_name = module_name;
  return true;
}

void SwordFuncs::versification(const bool on) { versenum = on; }
bool SwordFuncs::validModule() const noexcept { return module != nullptr; }

std::string SwordFuncs::currentRef() const {
  const std::string value = vkey.getText();
  return value.empty() ? "<EMPTY>" : value;
}

std::string SwordFuncs::currentText() {
  if (module == nullptr) throw std::runtime_error("No SWORD module is selected");
  module->setKey(vkey);
  std::ostringstream output;
  const std::string text = module->renderText().c_str();
  if (versenum) output << ' ' << vkey.getVerse();
  output << ' ' << trim(text);
  return output.str();
}

std::string SwordFuncs::parseInput(const std::string_view input) {
  const std::string command = trim(input);
  if (command.starts_with('?')) {
    throw std::invalid_argument("Search commands are not supported yet");
  }
  if (command.starts_with('!')) {
    const std::string requested = trim(std::string_view{command}.substr(1));
    if (requested.empty()) throw std::invalid_argument("Module name cannot be empty");
    if (!setModule(requested)) {
      throw std::invalid_argument("Unknown SWORD module '" + requested + "'");
    }
    return currentText();
  }
  if (command.empty()) {
    if (vkey.isTraversable()) ++vkey;
    return currentText();
  }
  return lookup(command);
}

std::string SwordFuncs::listModules() {
  if (manager == nullptr) return {};
  std::ostringstream output;
  for (const auto &[name, available_module] : manager->Modules) {
    output << '[' << name << "]\t - " << available_module->getDescription() << '\n';
  }
  return output.str();
}

const std::string &SwordFuncs::modname() const noexcept { return mod_name; }

std::string SwordFuncs::lookup(const std::string_view reference) {
  if (module == nullptr) throw std::runtime_error("No SWORD module is selected");
  ValidatingVerseKey parser;
  const std::string cleaned_reference = trim(reference);
  std::size_t book_end = cleaned_reference.size();
  for (std::size_t index = 0; index < cleaned_reference.size(); ++index) {
    const auto character = static_cast<unsigned char>(cleaned_reference[index]);
    if (std::isdigit(character) != 0 && index != 0) {
      book_end = index;
      break;
    }
  }
  const std::string book = trim(std::string_view{cleaned_reference}.substr(0, book_end));
  if (book.empty() || parser.getBookFromAbbrev(book.c_str()) < 0) {
    throw std::invalid_argument("Invalid Scripture reference '" + std::string{reference} + "'");
  }
  sword::ListKey range = parser.parseVerseList(cleaned_reference.c_str(), parser, true);
  if (range.getCount() == 0 || range.popError()) {
    throw std::invalid_argument("Invalid Scripture reference '" + std::string{reference} + "'");
  }
  range.setPersist(true);
  module->setKey(range);

  std::ostringstream output;
  int count = 0;
  for ((*module) = sword::TOP; !module->popError(); (*module)++) {
    ++count;
    const sword::VerseKey key{module->getKey()};
    const std::string text = module->renderText().c_str();
    if (versenum) output << ' ' << key.getVerse();
    output << ' ' << trim(text);
  }
  if (count == 0) throw std::invalid_argument("Invalid Scripture reference '" + std::string{reference} + "'");
  if (count > 1) output << '\n' << module->getKey()->getRangeText();
  vkey = module->getKey();
  // SWModule retains the key pointer. Restore the owned key before the local
  // ListKey is destroyed so module teardown never observes a dangling key.
  module->setKey(vkey);
  return output.str();
}

bool SwordFuncs::makeEntry(const std::string_view reference, const std::string_view input) {
  if (module == nullptr || !module->isWritable()) return false;
  vkey.setText(std::string{reference}.c_str());
  module->setKey(vkey);
  const std::string existing = currentText();
  const std::string data = trim(existing).empty()
                               ? std::string{input}
                               : existing + "<br/><br/>\n\n" + std::string{input};
  module->setEntry(data.c_str(), static_cast<std::int64_t>(data.size()));
  return true;
}

bool SwordFuncs::clearEntry(const std::string_view reference) {
  if (module == nullptr || !module->isWritable()) return false;
  vkey.setText(std::string{reference}.c_str());
  module->setKey(vkey);
  module->setEntry("", 0);
  return true;
}
