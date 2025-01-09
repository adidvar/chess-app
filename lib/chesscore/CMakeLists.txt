cmake_minimum_required(VERSION 3.29.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB core_sources
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB core_headers
     ${CMAKE_CURRENT_SOURCE_DIR}/include/chesscore/*.hpp
)
file(GLOB core_tests
     ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

add_library(chesscore ${core_headers} ${core_sources})

target_include_directories(chesscore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(chesscore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/chesscore)
target_include_directories(chesscore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

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

add_executable(test_chesscore ${core_tests})
target_include_directories(test_chesscore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/chesscore)
target_link_libraries(test_chesscore chesscore Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(test_chesscore)

endif()

