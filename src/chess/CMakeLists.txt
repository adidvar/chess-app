cmake_minimum_required(VERSION 3.22.1)

if(BUILD_UI)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB ui_sources
     *.hpp
     *.h
     *.cpp
     *.ui
)

add_executable(chess ${ui_sources})
target_link_libraries(chess PUBLIC Qt6::Widgets PUBLIC chesscore chessai)
target_include_directories(chess PUBLIC .)

endif()
