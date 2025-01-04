cmake_minimum_required(VERSION 3.22.1)

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

if(BUILD_TESTS)

#enable_testing()

#Include(FetchContent)

#FetchContent_Declare(
#  Catch2
#  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#  GIT_TAG        v3.0.1 # or a later release
#)

#FetchContent_MakeAvailable(Catch2)

#add_executable(test_chessai ${ai_tests})
#target_include_directories(test_chessai PUBLIC include/chesscore)
#target_link_libraries(test_chessai chessai Catch2::Catch2WithMain)
#add_test(NAME test_chessai COMMAND test_chessai)

endif()

