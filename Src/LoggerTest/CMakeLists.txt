include(Common)

project(LoggerTest)

link_libraries(GoogleTest
               Logger)

add_executable (${PROJECT_NAME}
                "LoggerTest.cpp")

