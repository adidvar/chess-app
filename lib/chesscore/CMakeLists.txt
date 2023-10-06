cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ChessCore)

file(GLOB core_sources
     src/*.hpp
     src/*.cpp
)
file(GLOB core_headers
     include/*.hpp
)

file(GLOB core_tests
     tests/*.cpp
)

add_library(chesscore ${core_headers} ${core_sources})
target_include_directories(chesscore PUBLIC include)

if(TESTING)

add_executable(chesscoretests ${core_tests})
target_link_libraries(chesscoretests chesscore Catch2::Catch2WithMain)
add_test(NAME chesscoretests COMMAND chesscoretests)

endif()

