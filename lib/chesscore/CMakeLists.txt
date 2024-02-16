cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB core_sources
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB core_headers
     ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

file(GLOB core_tests
     ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

add_library(chesscore ${core_headers} ${core_sources})
target_include_directories(chesscore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(TESTING)

enable_testing()

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(chesscoretests ${core_tests})
target_link_libraries(chesscoretests chesscore Catch2::Catch2WithMain)
add_test(NAME chesscoretests COMMAND chesscoretests)

endif()

