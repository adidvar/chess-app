cmake_minimum_required(VERSION 3.16.3)

project(ChessCore)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(BUILD_TESTING)
    enable_testing()



endif()

file(GLOB files
     "*.h"
     "*.cpp"
)

add_executable(BoardDebugger ${files})
target_link_libraries(BoardDebugger ChessCore Magic)

