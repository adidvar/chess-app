cmake_minimum_required(VERSION 3.16.3)

project(ChessCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(MagicNumbers magicnumbers.cpp magicnumbers.h)
add_executable(Generator generator.cpp)

target_link_libraries(Generator MagicNumbers)

target_include_directories(MagicNumbers PUBLIC  .)
