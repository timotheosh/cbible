#!/bin/bash
mkdir -p /usr/local/src/tests/build
cd /usr/local/src/tests/build
cmake ../
make
./cbibleTests
