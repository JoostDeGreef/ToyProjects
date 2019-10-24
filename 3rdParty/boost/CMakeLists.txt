include(Common)

project(boost VERSION 1.0.0 DESCRIPTION "boost framework")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory ("libs/filesystem")
add_subdirectory ("libs/iostreams")

target_compile_definitions(${PROJECT_NAME} 
    INTERFACE
        -DBOOST_ALL_NO_LIB=1)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


