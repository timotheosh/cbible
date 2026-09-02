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

`cbible` is a command-line and Emacs interface for Unix-like systems to
[The SWORD Project](https://crosswire.org/sword/). It looks up Scripture from
installed SWORD modules and reads or writes entries in writable commentary
modules such as `Personal`.

## Requirements

- A Unix-like system with POSIX terminal interfaces (`unistd.h` and `sys/ioctl.h`)
- CMake 3.20 or newer and a C++20-capable Clang (`clang++`) or GCC (`g++`)
- SWORD and GNU readline development packages
- pkg-config for dependency discovery
- Ninja for the supplied presets
- Emacs for the optional ERT tests and minor mode

cbible should compile on Unix-like systems that provide these dependencies.
A warning-free build has been reported on FreeBSD. Automated builds and tests
currently cover Ubuntu 24.04 and 26.04 with GCC and Clang; other platforms are
not yet covered by CI.

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
cbible -b KJV -r "John 3:16"
cbible -b ESV -n -r "Romans 8:28-30"
printf '%s\n' "My note" | cbible -b Personal -r "Psalm 1:1" -i
cbible -b Personal -r "Psalm 1:1" -e
```

Run `cbible -b KJV` without `-r` for the readline interface. Enter a reference to look
it up, an empty line to advance, `!MODULE` to switch modules, and `q` or `quit`
to exit. `?` and `??` are reserved for a future search feature and currently
produce an explicit unsupported-command error.

### Configuration (TOML)

Starting with 0.21, cbible reads `~/.cbible.toml`. Copy
[examples/cbible.toml](examples/cbible.toml) there and select an installed module:

```toml
bible_version = "KJV"
```

`bible_version` is optional and must be a non-empty string. It is the only
supported top-level key; unknown keys, wrong types, and malformed TOML are
reported as configuration errors (exit 2). An explicit `-b` takes precedence,
but configuration files are still validated. Use `-c PATH` to read another
TOML file, regardless of its extension. A missing default file is fine; a
missing explicitly requested file is an error. Help and version do not read
configuration.

There is no implicit KJV default. Without a version in either configuration
or `-b`, cbible lists installed Bible versions and exits successfully, even
when `-r` is supplied. With an unknown module it prints an error and the list
to stderr and exits 3. Only Biblical Text modules appear in the list;
commentaries such as Personal remain usable with `-b Personal`.

Migration: `~/.cbible.cfg` and INI syntax are no longer supported. Replace
`bibleversion=KJV` with `bible_version = "KJV"` in `~/.cbible.toml`.
The header-only toml++ v3.4.0 parser and its MIT license are vendored in
`src/thirdparty`; no additional system dependency is needed.

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

- POSIX terminal interfaces are required; native Windows builds are not supported.
- Search, structured output, and a TUI/GUI are not implemented.
- SWORD and readline remain system dependencies.
- Releases are source-only; binary packages are not published.
