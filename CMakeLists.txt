cmake_minimum_required(VERSION 3.22.1)

project(chess-app LANGUAGES C CXX VERSION 0.1)

include(GNUInstallDirs)
enable_testing()

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "Chess-app")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}-v${CPACK_PACKAGE_VERSION}")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")

include(CPack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native -funroll-loops -fprefetch-loop-arrays")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /Oi /Gy /GF /EHsc")
endif()

option(BUILD_UCI "build UCI module" ON)
option(BUILD_TESTS "build tests" OFF)
option(BUILD_DEBUG_EXECUTABLE "build dev only executable" OFF)

file(GLOB libs LIST_DIRECTORIES TRUE lib/*)

subdirs(${libs})

file(GLOB bins LIST_DIRECTORIES TRUE src/*)

subdirs(${bins})

if(BUILD_TESTS)
    include(CTest)
endif()
