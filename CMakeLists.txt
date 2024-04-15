cmake_minimum_required(VERSION 3.22.1)

project(Chess5 LANGUAGES C CXX VERSION 5.0.0.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_UI "build UI module" OFF)
option(BUILD_UCI "build UCI module" OFF)
option(BUILD_TESTS "build tests" OFF)
option(BUILD_DEBUG_EXECUTABLE "build dev only executable" OFF)

file(GLOB libs LIST_DIRECTORIES TRUE lib/*)

subdirs(${libs})

file(GLOB bins LIST_DIRECTORIES TRUE src/*)

subdirs(${bins})
