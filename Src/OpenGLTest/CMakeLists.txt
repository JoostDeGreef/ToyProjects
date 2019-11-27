include(Common)

project(OpenGLTest)

link_libraries(OpenGL
               Logger
               ${CMAKE_DL_LIBS})

add_executable(${PROJECT_NAME}
               "Main.cpp"
               "UI.cpp")

