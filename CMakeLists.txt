cmake_minimum_required(VERSION 3.5.1)

# project name
project(plexus-effect)

# creates the variable EXEC and sets it to hello
set(EXEC plexus-effect)

# set the C++20 standard
set(CMAKE_CXX_STANDARD 20)

# for sdl2 on windows
IF(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -IC:/Development/DefaultLibs/SDL2-2.0.20/x86_64-w64-mingw32/include/")
ENDIF()

# I../include
# L../lib
include_directories(include)
include_directories(src)

add_subdirectory(src)
link_directories(lib)