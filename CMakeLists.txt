cmake_minimum_required(VERSION 3.22.1)

project(Chess5 LANGUAGES C CXX VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /Oi /Gy /GF /EHsc")
endif()

option(BUILD_UI "build UI module" ON)
option(BUILD_UCI "build UCI module" ON)
option(BUILD_TESTS "build tests" OFF)
option(BUILD_DEBUG_EXECUTABLE "build dev only executable" OFF)

file(GLOB libs LIST_DIRECTORIES TRUE lib/*)

subdirs(${libs})

file(GLOB bins LIST_DIRECTORIES TRUE src/*)

subdirs(${bins})
