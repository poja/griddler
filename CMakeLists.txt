cmake_minimum_required(VERSION 3.8)
project(griddler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_VERBOSE_MAKEFILE ON)

set(SOURCE_FILES main.cpp GriddlerPuzzle.cpp GriddlerPuzzle.h Line.cpp Line.h CnfFormula.cpp CnfFormula.h cxxopts.h)
add_executable(griddler ${SOURCE_FILES})