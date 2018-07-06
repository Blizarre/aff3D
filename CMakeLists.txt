cmake_minimum_required (VERSION 2.8)
project(Aff3D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

link_libraries (${SDL2_LIBRARIES})

include_directories(include)
file( GLOB
    sources
    src/*.cpp
    src/IO/*.cpp
    )

include_directories(${SDL2_INCLUDE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

ADD_EXECUTABLE(aff3D ${sources})
