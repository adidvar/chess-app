cmake_minimum_required(VERSION 3.16.3)

project(AI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB files
     "*.hpp"
     "*.cpp"
)

add_library(AI ${files})

target_include_directories(AI PUBLIC  .)

target_link_libraries(AI ChessCore Magic)

add_subdirectory("Benchmark")


