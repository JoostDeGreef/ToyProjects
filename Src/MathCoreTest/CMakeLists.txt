include(Common)

project(MathCoreTest)

link_libraries(GoogleTest
               MathCore)

add_executable (${PROJECT_NAME}
                "MatrixTest.cpp"
                "MatrixStaticTest.cpp"
                "VectorTest.cpp"
                "QuaternionTest.cpp"
                "MathCoreTest.cpp")

