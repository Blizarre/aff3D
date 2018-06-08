#!/bin/sh

set -e

SCRIPT_DIR="$(dirname "$0")"
find "$SCRIPT_DIR" -iname "*.cpp" -or -iname "*.h" -exec clang-format -i \{\} \;
