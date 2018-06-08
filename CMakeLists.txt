cmake_minimum_required (VERSION 2.8)
project(Aff3D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

link_libraries (${SDL2_LIBRARIES})

include_directories(include)
file( GLOB
    sources
    src/*.cpp
    src/IO/*.cpp
    )

include_directories(${SDL2_INCLUDE_DIR})

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
endif()

ADD_EXECUTABLE(aff3D ${sources})
