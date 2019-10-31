include(Common)

project(OpenGL VERSION 1.0.0 DESCRIPTION "OpenGL functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

link_libraries(MathCore
               boost
               GLAD
               GLFW
               FREETYPE)

add_library (${PROJECT_NAME} SHARED 
             "src/OpenGL.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

