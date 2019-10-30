include(Common)

project(OpenGLTest)

link_libraries(OpenGL)

add_executable(${PROJECT_NAME}
               "Main.cpp"
               "UI.cpp")

