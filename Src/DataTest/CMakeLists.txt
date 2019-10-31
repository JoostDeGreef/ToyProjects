include(Common)

project(DataTest)

link_libraries(GoogleTest
               Data)

add_executable (${PROJECT_NAME}
                "DataTest.cpp")

