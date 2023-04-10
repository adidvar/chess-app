cmake_minimum_required(VERSION 3.16.3)

project(Chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("Core")
add_subdirectory("UI")
#add_subdirectory("Net")
add_subdirectory("AI")
add_subdirectory("Debuging")

add_executable(Chess main.cpp)
target_link_libraries(Chess ChessCore ChessUI)
