cmake_minimum_required(VERSION 3.9)

project(MathCore VERSION 1.0.0 DESCRIPTION "Core math functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Why doesn't SHARED work?
add_library (${PROJECT_NAME}
             "src/MathCore.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/src-extra
)

