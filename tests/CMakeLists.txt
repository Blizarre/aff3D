cmake_minimum_required (VERSION 2.8)
project(Tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

link_libraries (${SDL2_LIBRARIES} pthread)

add_definitions(-DUNIT_TESTS)

include_directories(../include googletest/include ../src ../src/IO ../include/IO googletest)
file( GLOB
    sources
    *.cpp
    ../src/*.cpp
    ../src/IO/*.cpp
    googletest/src/gtest-all.cc
    googletest/src/gtest_main.cc
    )

include_directories(${SDL2_INCLUDE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

ADD_EXECUTABLE(aff3D-test ${sources})
