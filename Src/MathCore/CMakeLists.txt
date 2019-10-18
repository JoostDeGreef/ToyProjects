cmake_minimum_required (VERSION 3.8)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(MathCore)

add_library (${PROJECT_NAME} SHARED 
             "src/MathCore.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

#how this should be done: https://rix0r.nl/blog/2015/08/13/cmake-guide/
#export(TARGETS
#        ${PROJECT_NAME}
#    NAMESPACE ToyProjects::
#    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#)

