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

add_library(chessai ${ai_headers} ${ai_sources})
target_link_libraries(chessai chesscore)
target_include_directories(chessai PUBLIC include)

install(TARGETS chessai)
INSTALL(FILES ${ai_headers}
        DESTINATION include/chessai
)

install(TARGETS chessai)

if(BUILD_TESTS)

enable_testing()

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(chessaitests ${ai_tests})
target_include_directories(chessaitests PUBLIC include/chesscore)
target_link_libraries(chessaitests chessai Catch2::Catch2WithMain)
add_test(NAME chessaitests COMMAND chesscoretests)

endif()

