cmake_minimum_required(VERSION 3.16)

project(ChessUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

file(GLOB files
     "*.h"
     "*.cpp"
     "*.ui"
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

add_library(ChessUI ${files})
target_link_libraries(ChessUI PUBLIC Qt6::Widgets PUBLIC ChessCore)
target_include_directories(ChessUI PUBLIC .)
