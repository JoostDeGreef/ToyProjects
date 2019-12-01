include(Common)

project(Geometry VERSION 1.0.0 DESCRIPTION "Geometry functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library (${PROJECT_NAME}
             "src/Geometry.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

