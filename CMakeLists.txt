cmake_minimum_required(VERSION 3.25.1)

project(Chess LANGUAGES C CXX VERSION 5.0.0.0)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB libs LIST_DIRECTORIES TRUE lib/*)

subdirs(${libs})

file(GLOB bins LIST_DIRECTORIES TRUE src/*)

subdirs(${bins})
