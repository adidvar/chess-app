cmake_minimum_required(VERSION 3.16.3)

if(BUILD_DEBUG_EXECUTABLE)

file(GLOB files
     "*.hpp"
     "*.cpp"
)

add_executable(debug ${files})
target_link_libraries(debug chessai chesscore)

endif()


