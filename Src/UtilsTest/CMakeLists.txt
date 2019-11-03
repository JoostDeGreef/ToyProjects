include(Common)

project(UtilsTest)

link_libraries(GoogleTest
               Utils)

add_executable (${PROJECT_NAME}
                "GridTest.cpp")

