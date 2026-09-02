#include "Options.hpp"
#include "Output.hpp"
#include "SwordFuncs.hpp"
#include "utilities.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

namespace {
Options parse(std::vector<std::string> arguments) {
  std::vector<char *> argv;
  argv.reserve(arguments.size());
  for (auto &argument : arguments) argv.push_back(argument.data());
  return {static_cast<int>(argv.size()), argv.data()};
}
} // namespace

TEST_CASE("string utilities are safe and deterministic") {
  CHECK(trim(" \t text \n") == "text");
  CHECK(trim(" \t\n").empty());
  CHECK(wrapText("one two three", 7) == "one\ntwo\nthree");
  CHECK(wrapText("unchanged", 0) == "unchanged");
}

TEST_CASE("options preserve defaults and validate combinations") {
  CHECK(parse({"cbible", "-r", "Gen 1:1"}).getOption("bibleversion") == "KJV");
  CHECK(parse({"cbible", "-b", "Personal", "-r", "Gen 1:1"}).getOption("bibleversion") == "Personal");
  CHECK(parse({"cbible", "--bibleversion=Personal", "--reference=Gen 1:1"}).getOption("reference") == "Gen 1:1");
  CHECK_FALSE(parse({"cbible", "--input"}).valid());
  CHECK_FALSE(parse({"cbible", "--empty"}).valid());
  CHECK_FALSE(parse({"cbible", "--input", "--empty", "-r", "Gen 1:1"}).valid());
  CHECK_FALSE(parse({"cbible", "--unknown"}).valid());
}

TEST_CASE("SWORD lookup preserves legacy output") {
  SwordFuncs sword{"KJV"};
  REQUIRE(sword.validModule());
  CHECK(sword.currentRef() == "Genesis 1:1");
  CHECK(sword.lookup("Rom 8:28") ==
        " 28 And we know that all things work together for good to them that love God, to them who are the called according to his purpose.");
  sword.versification(false);
  CHECK(sword.lookup("Gen 1:1") == " In the beginning God created the heaven and the earth.");
  CHECK_THROWS_AS(sword.lookup("not-a-reference"), std::invalid_argument);
}

TEST_CASE("failed module switch is transactional") {
  SwordFuncs sword{"KJV"};
  CHECK_THROWS_AS(sword.parseInput("!DoesNotExist"), std::invalid_argument);
  CHECK(sword.modname() == "KJV");
  CHECK(sword.lookup("Gen 1:1").find("In the beginning") != std::string::npos);
}

TEST_CASE("interactive core advances and reserves search commands") {
  SwordFuncs sword{"KJV"};
  sword.parseInput("Gen 1:1");
  CHECK(sword.parseInput("").find("And the earth was without form") != std::string::npos);
  CHECK_THROWS_AS(sword.parseInput("?faith"), std::invalid_argument);
  CHECK_THROWS_AS(sword.parseInput("??faith"), std::invalid_argument);
}

TEST_CASE("Personal commentary can be written and cleared") {
  SwordFuncs sword{"Personal"};
  REQUIRE(sword.validModule());
  sword.versification(false);
  const std::string reference = "Lam 3:4";
  REQUIRE(sword.clearEntry(reference));
  REQUIRE(sword.makeEntry(reference, "A test note.\nSecond line."));
  CHECK(sword.lookup(reference) == " A test note.\nSecond line.");
  CHECK(sword.clearEntry(reference));
}

TEST_CASE("read-only modules reject commentary changes") {
  SwordFuncs sword{"KJV"};
  CHECK_FALSE(sword.makeEntry("Gen 1:1", "note"));
  CHECK_FALSE(sword.clearEntry("Gen 1:1"));
}
