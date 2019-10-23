cmake_minimum_required(VERSION 3.9)

project(GoogleTest VERSION 1.0.0 DESCRIPTION "Google test framework")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library (${PROJECT_NAME} SHARED 
             "src/gtest/gtest-all.cc"
             "src-extra/ConsoleOutput.cpp"
             "src-extra/ConsoleOutput.h"
             "src-extra/GoogleTestMain.cpp"
             "src-extra/VisualStudioOutput.cpp"
             "src-extra/VisualStudioOutput.h")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src-extra
)

