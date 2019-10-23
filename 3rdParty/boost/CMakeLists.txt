cmake_minimum_required(VERSION 3.9)

project(boost VERSION 1.0.0 DESCRIPTION "boost framework")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

#add_custom_target(${PROJECT_NAME} DEPENDS Boost-FileSystem)
add_library(${PROJECT_NAME} INTERFACE)

# Include sub-projects.
add_subdirectory ("libs/filesystem")

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/src-extra
)


