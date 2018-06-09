#!/bin/sh

set -e

SCRIPT_DIR="$(dirname "$0")"
find "$SCRIPT_DIR" \
  -iname "*.cpp" -or -iname "*.h" -or -iname "*.hpp" \
  -exec clang-format -i '{}' \;
