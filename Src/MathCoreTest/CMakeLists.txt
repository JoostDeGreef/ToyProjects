include(Common)

project(MathCoreTest)

link_libraries(GoogleTest
               MathCore)

add_executable (${PROJECT_NAME}
                "VectorTest.cpp"
                "QuaternionTest.cpp"
                "MathCoreTest.cpp"
                "MatrixTest.cpp")

