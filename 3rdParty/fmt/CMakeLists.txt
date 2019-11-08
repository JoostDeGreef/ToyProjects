include(Common)

project(Format VERSION 1.0.0 DESCRIPTION "formatting utilities")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory("fmt")

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/fmt/include>)

target_link_libraries(${PROJECT_NAME} INTERFACE fmt-header-only)



