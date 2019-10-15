# CMakeList.txt : CMake project for MathCore, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

link_libraries(GoogleTest 
			   MathCore)

# Add source to this project's executable.
add_executable (MathCoreTest
                "MathCoreTest.cpp")

# TODO: Add tests and install targets if needed.
