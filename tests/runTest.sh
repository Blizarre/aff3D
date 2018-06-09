#!/bin/sh

set -e

curl -L https://github.com/google/googletest/archive/release-1.8.0.tar.gz | tar -zx --strip-components=1  googletest-release-1.8.0/googletest/
mkdir -p build
cd build
cmake ..
make -j4
./aff3D-test
