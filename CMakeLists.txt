# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ToyProjects")

include_directories("3rdParty/include"
                    "Src/MathCore/include")

# Include sub-projects.
add_subdirectory ("Src")
add_subdirectory ("3rdParty")
