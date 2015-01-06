cmake_minimum_required (VERSION 2.8) 
project(Aff3D)

find_package(SDL REQUIRED)

if(NOT WIN32)
	find_library(MATH m)
else()
	set(MATH "")
endif()

link_libraries (
       ${SDL_LIBRARY}
       ${MATH_LIBRARY}
)

include_directories(include)
file( GLOB
    sources
    src/*.cpp
    )

include_directories(${SDL_INCLUDE_DIR})

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

ADD_EXECUTABLE(aff3D ${sources})