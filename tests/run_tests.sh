#!/usr/bin/env bash
set -euo pipefail
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
