cmake_minimum_required(VERSION 3.22.1)

if(BUILD_UCI)

file(GLOB files
     "*.hpp"
     "*.cpp"
)

#add_executable(uci ${files})

#target_include_directories(uci PRIVATE .)

#target_link_libraries(uci chesscore chessai)

endif()


