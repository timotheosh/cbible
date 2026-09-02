#!/usr/bin/env bash
set -euo pipefail

binary=$1
fixture=$2
export SWORD_PATH="$fixture"
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT
export HOME="$test_dir"

[[ "$($binary --version)" == "cbible Version 0.21" ]]
"$binary" --help | grep -q -- '--bibleversion'
[[ "$($binary -b KJV -r 'Gen 1:1')" == " In the beginning God created the heaven and the earth." ]]
[[ "$($binary -n -b KJV -r 'Gen 1:1')" == " 1 In the beginning God created the heaven and the earth." ]]

if "$binary" --unknown >"$test_dir/out" 2>"$test_dir/err"; then exit 1; fi
[[ ! -s "$test_dir/out" ]]
grep -q "Unknown option" "$test_dir/err"

if "$binary" -i >"$test_dir/out" 2>"$test_dir/err"; then exit 1; fi
grep -q "requires --reference" "$test_dir/err"

if "$binary" -b KJV -r 'not-a-reference' >"$test_dir/out" 2>"$test_dir/err"; then exit 1; fi
[[ ! -s "$test_dir/out" ]]
grep -q "Invalid Scripture reference" "$test_dir/err"

if printf 'note' | "$binary" -b KJV -r 'Gen 1:1' -i >"$test_dir/out" 2>"$test_dir/err"; then exit 1; fi
grep -q "not writable" "$test_dir/err"

# No implicit KJV selection, and commentary modules are not Bible versions.
"$binary" >"$test_dir/out" 2>"$test_dir/err"
grep -q 'KJV - ' "$test_dir/out"
! grep -q Personal "$test_dir/out"
[[ ! -s "$test_dir/err" ]]
status=0
"$binary" -b DoesNotExist >"$test_dir/out" 2>"$test_dir/err" || status=$?
[[ "$status" == 3 && ! -s "$test_dir/out" ]]
grep -q 'Unknown SWORD module' "$test_dir/err"
grep -q 'KJV - ' "$test_dir/err"

# TOML is used regardless of filename; -b overrides a valid configuration.
printf '%s\n' 'bible_version = "KJV"' >"$HOME/.cbible.toml"
[[ "$($binary -r 'Gen 1:1')" == " In the beginning God created the heaven and the earth." ]]
printf '%s\n' 'bible_version = "DoesNotExist"' >"$test_dir/custom.cfg"
[[ "$($binary -c "$test_dir/custom.cfg" -b KJV -r 'Gen 1:1')" == " In the beginning God created the heaven and the earth." ]]

for contents in 'bible_version = [' 'bible_version = 12' 'bible_version = ""' 'bible_version = "  "' 'unknown = "KJV"' 'bibleversion=KJV'; do
  printf '%s\n' "$contents" >"$test_dir/custom.cfg"
  status=0
  "$binary" -c "$test_dir/custom.cfg" -b KJV >"$test_dir/out" 2>"$test_dir/err" || status=$?
  [[ "$status" == 2 && ! -s "$test_dir/out" && -s "$test_dir/err" ]]
  grep -q 'custom.cfg' "$test_dir/err"
done
status=0
"$binary" -c "$test_dir/missing" >"$test_dir/out" 2>"$test_dir/err" || status=$?
[[ "$status" == 2 && ! -s "$test_dir/out" ]]
"$binary" -c "$test_dir/missing" -h >"$test_dir/out"
"$binary" -c "$test_dir/missing" -v >"$test_dir/out"

# Legacy INI is not consulted. Empty TOML leaves the version unspecified.
printf '%s\n' 'bibleversion=DoesNotExist' >"$HOME/.cbible.cfg"
printf '%s\n' '# No default module' >"$HOME/.cbible.toml"
"$binary" -r 'Gen 1:1' >"$test_dir/out"
grep -q 'Installed Bible versions:' "$test_dir/out"
