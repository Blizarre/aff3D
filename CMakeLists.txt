cmake_minimum_required (VERSION 2.6) 
project(Aff3D)

find_package(SDL REQUIRED)

link_libraries (
       ${SDL_LIBRARY}
       m
)

include_directories(include)
file( GLOB
    sources
    src/*.cpp
    )

include_directories(${SDL_INCLUDE_DIR})

ADD_EXECUTABLE(aff3D ${sources})


