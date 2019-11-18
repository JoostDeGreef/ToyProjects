include(Common)

project(MathCoreTest)

link_libraries(GoogleTest
               MathCore)

add_executable (${PROJECT_NAME}
                "MatrixTest.cpp"
                "MatrixFixedTest.cpp"
                "VectorTest.cpp"
                "QuaternionTest.cpp"
                "MathCoreTest.cpp")

