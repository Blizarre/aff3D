#!/bin/sh

set -e

cd "$(dirname "$0")"
mkdir -p build_scan && cd build_scan
scan-build -v cmake -DCMAKE_BUILD_TYPE=Debug ..
scan-build -v make clean aff3D

