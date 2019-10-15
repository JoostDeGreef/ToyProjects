# CMakeList.txt : CMake project for MathCore, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (MathCore 
             "src/MathCore.cpp"
			 "include/MathCore.h")

# TODO: Add tests and install targets if needed.
