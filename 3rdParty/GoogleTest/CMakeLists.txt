# CMakeList.txt : CMake project for MathCore, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (GoogleTest 
             "src/gtest/gtest-all.cc"
             "src-extra/ConsoleOutput.cpp"
             "src-extra/ConsoleOutput.h"
             "src-extra/GoogleTestMain.cpp"
             "src-extra/OutputTest.cpp"
             "src-extra/VisualStudioOutput.cpp"
             "src-extra/VisualStudioOutput.h")

# TODO: Add tests and install targets if needed.
