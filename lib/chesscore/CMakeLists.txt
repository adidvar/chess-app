cmake_minimum_required(VERSION 3.30.0)

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

if(BUILD_TESTS)
enable_testing()
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)
add_executable(test_chesscore ${core_tests})
target_include_directories(test_chesscore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/chesscore)
target_link_libraries(test_chesscore chesscore Catch2::Catch2WithMain)
add_test(NAME test_chesscore COMMAND test_chesscore)
endif()

