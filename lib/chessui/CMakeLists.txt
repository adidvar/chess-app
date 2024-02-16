cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

file(GLOB ui_sources
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui
)
file(GLOB ui_headers
     ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

file(GLOB ui_tests
     ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

add_library(chessui ${ui_headers} ${ui_sources})
target_link_libraries(chessui PUBLIC Qt6::Widgets PUBLIC chesscore chessai)
target_include_directories(chessui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)






