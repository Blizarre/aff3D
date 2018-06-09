cmake_minimum_required (VERSION 2.8)
project(Tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

link_libraries (${SDL2_LIBRARIES} pthread)

include_directories(../include googletest/include googletest)
include_directories()
file( GLOB
    sources
    *.cpp
    ../src/*.cpp
    ../src/IO/*.cpp
    googletest/src/gtest-all.cc
    googletest/src/gtest_main.cc
    )

include_directories(${SDL2_INCLUDE_DIR})

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_MAIN -Wall -Werror -std=c++14")
endif()

ADD_EXECUTABLE(aff3D-test ${sources})
