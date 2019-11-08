include(Common)

project(Logger VERSION 1.0.0 DESCRIPTION "Logging library")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

link_libraries(Format
               boost
               Boost-FileSystem)

add_library (${PROJECT_NAME} # SHARED
             "src/Logger.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


