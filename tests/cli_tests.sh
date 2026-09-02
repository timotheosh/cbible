#!/usr/bin/env bash
set -euo pipefail

binary=$1
fixture=$2
export SWORD_PATH="$fixture"

[[ "$($binary --version)" == "cbible Version 0.20" ]]
"$binary" --help | grep -q -- '--bibleversion'
[[ "$($binary -b KJV -r 'Gen 1:1')" == " In the beginning God created the heaven and the earth." ]]
[[ "$($binary -n -b KJV -r 'Gen 1:1')" == " 1 In the beginning God created the heaven and the earth." ]]

if "$binary" --unknown >/tmp/cbible-test-out 2>/tmp/cbible-test-err; then exit 1; fi
[[ ! -s /tmp/cbible-test-out ]]
grep -q "Unknown option" /tmp/cbible-test-err

if "$binary" -i >/tmp/cbible-test-out 2>/tmp/cbible-test-err; then exit 1; fi
grep -q "requires --reference" /tmp/cbible-test-err

if "$binary" -b KJV -r 'not-a-reference' >/tmp/cbible-test-out 2>/tmp/cbible-test-err; then exit 1; fi
[[ ! -s /tmp/cbible-test-out ]]
grep -q "Invalid Scripture reference" /tmp/cbible-test-err

if printf 'note' | "$binary" -b KJV -r 'Gen 1:1' -i >/tmp/cbible-test-out 2>/tmp/cbible-test-err; then exit 1; fi
grep -q "not writable" /tmp/cbible-test-err
