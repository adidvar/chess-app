cmake_minimum_required(VERSION 3.22.1)

if(BUILD_UI)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

file(GLOB ui_sources
     *.hpp
     *.h
     *.cpp
     *.ui
)

add_executable(chess ${ui_sources})
target_link_libraries(chess PUBLIC Qt6::Widgets PUBLIC chesscore chessai)

qt_generate_deploy_app_script(
    TARGET chess
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

install(TARGETS chess)

endif()
