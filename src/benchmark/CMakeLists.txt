cmake_minimum_required(VERSION 3.16.3)

project(Benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB files
     "*.hpp"
     "*.cpp"
)

add_executable(benchmark ${files})

target_link_libraries(benchmark chessai)


