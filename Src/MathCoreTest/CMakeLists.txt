include(Common)

project(MathCoreTest)

link_libraries(GoogleTest
               MathCore)

add_executable (${PROJECT_NAME}
                "VectorTest.cpp"
                "QuaternionTest.cpp"
                "MathCoreTest.cpp"
                "MatrixFixedTest.cpp"
                "MatrixTest.cpp")

