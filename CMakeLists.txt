
cmake_minimum_required(VERSION 3.16.3)

project(Chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Chess main.cpp board.cpp bot.cpp map_preview.cpp player.cpp turn.cpp turnexecutor.cpp turngenerator.cpp )

target_include_directories(Chess PUBLIC
                          "/usr/include/SFML"
                          )
target_link_directories(Chess PUBLIC "/usr/lib/x86_64-linux-gnu")

target_link_libraries(Chess 
						sfml-window
						sfml-system
						sfml-graphics
						pthread
						)
						
target_compile_options(Chess PUBLIC "-O3")
