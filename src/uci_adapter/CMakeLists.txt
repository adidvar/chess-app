cmake_minimum_required(VERSION 3.16.3)

project(UciAdapter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB files
     "*.hpp"
     "*.cpp"
)

#add_executable(uci_adapter ${files})

#target_include_directories(uci_adapter PUBLIC  .)

#target_link_libraries(uci_adapter chesscore chessai)


