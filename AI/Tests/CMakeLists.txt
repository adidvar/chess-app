cmake_minimum_required(VERSION 3.16.3)

project(AI)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(BUILD_TESTING)
    enable_testing()

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

file(GLOB files
    "*.h"
    "*.cpp"
)

add_executable(AITests ${files})
target_link_libraries(AITests AI Catch2::Catch2WithMain)
add_test(NAME AITests COMMAND AITests)

endif()
