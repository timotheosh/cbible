```text
      __          __       ___
     /\ \      __/\ \     /\_ \
  ___\ \ \____/\_\ \ \____\//\ \      __
 /'___\ \ '__`\/\ \ \ '__`\ \ \ \   /'__`\
/\ \__/\ \ \L\ \ \ \ \ \L\ \ \_\ \_/\  __/
\ \____\\ \_,__/\ \_\ \_,__/ /\____\ \____\
 \/____/ \/___/  \/_/\/___/  \/____/\/____/
```

# cbible

`cbible` is a Linux command-line and Emacs interface to
[The SWORD Project](https://crosswire.org/sword/). It looks up Scripture from
installed SWORD modules and reads or writes entries in writable commentary
modules such as `Personal`.

## Requirements

- Linux (Ubuntu 24.04 is the stable CI baseline; Ubuntu 26.04 is tested while
  its GitHub runner remains in preview)
- CMake 3.20 or newer and a C++20 compiler
- SWORD and GNU readline development packages
- Ninja for the supplied presets
- Emacs for the optional ERT tests and minor mode

On Ubuntu:

```sh
sudo apt-get install cmake ninja-build clang libsword-dev libreadline-dev emacs-nox
```

Bible texts are not bundled. Install modules with a SWORD-compatible module
manager. Tests use the small fixtures checked into `tests/swordtest`.

## Build and test

The preferred developer compiler is Clang:

```sh
CXX=clang++ cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Use `CXX=g++` for GCC. If `CXX` is omitted, CMake uses the platform default.
Other useful presets are `release` and `sanitizers`. A conventional build also
works:

```sh
cmake -S . -B build/local -DCMAKE_BUILD_TYPE=Release
cmake --build build/local
ctest --test-dir build/local --output-on-failure
cmake --install build/local
```

## Command-line use

```text
cbible -r "John 3:16"
cbible -b ESV -n -r "Romans 8:28-30"
printf '%s\n' "My note" | cbible -b Personal -r "Psalm 1:1" -i
cbible -b Personal -r "Psalm 1:1" -e
```

Run `cbible` without `-r` for the readline interface. Enter a reference to look
it up, an empty line to advance, `!MODULE` to switch modules, and `q` or `quit`
to exit. `?` and `??` are reserved for a future search feature and currently
produce an explicit unsupported-command error.

Configuration remains compatible with earlier versions. By default cbible
reads `~/.cbible.cfg`:

```ini
bibleversion=KJV
```

An explicit `-b` takes precedence. Use `-c PATH` to read another config file.

## Emacs mode

Install `elisp/cbible.el` into your load path and add:

```elisp
(require 'cbible)
(setq cbible-bible-version "KJV")
```

Enable `cbible-mode`; `C-c l` prompts for a module and reference and inserts the
result. `cbible-entry-region` and `cbible-entry-buffer` write Personal
commentary. The integration invokes cbible directly, so references and notes
are never evaluated by a shell. Customize `cbible-program` if the executable is
not on `exec-path`.

## Current limitations

- Linux is the supported platform; terminal-specific code is isolated for
  future portability work.
- Search, structured output, and a TUI/GUI are not implemented.
- SWORD and readline remain system dependencies.
- This repository does not currently publish binary packages or release
  artifacts.
