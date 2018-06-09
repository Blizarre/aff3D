#!/bin/sh

set -e

cd "$(dirname "$0")"
if test ! -f compile_commands.json; then
  echo "The compilation database has not been found in the root directory."
  echo "CMake should generate one for you the the build directory. Link it to:"
  echo "<repo_root>/compile_commands.json"
  exit 1
fi
find src -iname "*.cpp" -exec clang-tidy '{}' \;
