include(Common)

project(GeometryTest)

link_libraries(GoogleTest
               MathCore
               Geometry)

add_executable (${PROJECT_NAME}
                "Line2dTest.cpp"
                "Line3dTest.cpp"
                "GeometryTest.cpp")

