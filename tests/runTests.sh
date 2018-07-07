#!/bin/sh

set -e

cd "$(dirname "$0")"

curl -L https://github.com/google/googletest/archive/release-1.8.0.tar.gz | tar -zx --strip-components=1  googletest-release-1.8.0/googletest/
mkdir -p build
cd build
cmake ..
make -j
./aff3D-test
