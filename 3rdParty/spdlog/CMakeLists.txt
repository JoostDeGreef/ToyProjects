include(Common)

project(SPDLog VERSION 1.0.0 DESCRIPTION "logging framework")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory ("spdlog")

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include>)


