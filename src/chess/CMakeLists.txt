cmake_minimum_required(VERSION 3.16.3)

project(Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB files
     "*.hpp"
     "*.cpp"
)

add_executable(Chess ${files})
target_link_libraries(Chess chessai chessui chesscore)


