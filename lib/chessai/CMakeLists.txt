cmake_minimum_required(VERSION 3.29.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB ai_sources
     src/*.hpp
     src/*.cpp
)
file(GLOB ai_headers
     include/*.hpp
)

file(GLOB ai_tests
     tests/*.test.cpp
)

#add_library(chessai ${ai_headers} ${ai_sources})
#target_link_libraries(chessai chesscore)
#target_include_directories(chessai PUBLIC include)
