cmake_minimum_required(VERSION 3.16.3)

project(ChessCore)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(BUILD_TESTING)
    enable_testing()

add_executable(BoardDebugger board_debugging.cpp)
target_link_libraries(BoardDebugger ChessCore Magic)

endif()
