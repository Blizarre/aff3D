#!/bin/sh

set -e

if test ! -f compile_commands.json; then
  echo "The compilation database (compile_commands.json) has not been found in
  echo "the current directory. You should execute this script from the Cmake
  echo "build directory."
  exit 1
fi

find "$(dirname "$0")/src" -iname "*.cpp" -exec clang-tidy '{}' \;
