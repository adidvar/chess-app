cmake_minimum_required(VERSION 3.29.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB ai_sources
     src/*.hpp
     src/*.cpp
)
file(GLOB ai_headers
     include/chessai/*.hpp
)
file(GLOB ai_tests
     tests/*.test.cpp
)

add_library(chessai ${ai_headers} ${ai_sources})

target_link_libraries(chessai chesscore)
target_include_directories(chessai PUBLIC include)
target_include_directories(chessai PRIVATE include/chessai)

if(BUILD_TESTS)
enable_testing()

find_package(Catch2 3 QUIET)

if (NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_SHALLOW    TRUE
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.0)
    FetchContent_MakeAvailable(catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

add_executable(test_chessai ${ai_tests})
target_include_directories(test_chessai PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(test_chessai chessai Catch2::Catch2WithMain)

#include(CTest)
#include(Catch)
#catch_discover_tests(test_chesscore)

endif()
