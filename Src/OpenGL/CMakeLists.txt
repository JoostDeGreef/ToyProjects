include(Common)

project(OpenGL)

link_libraries(MathCore
               GLAD
			   GLFW
			   FREETYPE)

add_library (${PROJECT_NAME} SHARED 
             "src/OpenGL.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

