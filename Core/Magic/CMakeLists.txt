cmake_minimum_required(VERSION 3.16.3)

project(ChessCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB files
     "*.h"
     "*.cpp"
)

add_library(Magic ${files})

target_include_directories(Magic PUBLIC  .)