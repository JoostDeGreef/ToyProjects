# CMakeList.txt : CMake project for MathCore, include source and define
# project specific logic here.
#
include(Library)

# Add source to this project's executable.
add_library (MathCore SHARED 
             "src/MathCore.cpp"
			 "include/MathCore.h")

#install(TARGETS MathCore DESTINATION ../../bin)
