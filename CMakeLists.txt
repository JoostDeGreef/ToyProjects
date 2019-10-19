# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# These are just toy projects, make that clear
project ("ToyProjects")

# All include directories
include_directories("3rdParty/include")

# Include sub-projects.
add_subdirectory ("Src")
add_subdirectory ("3rdParty")

# Output will land here
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_BUILD_TYPE} CACHE
    PATH "Directory where all the .lib files are dumped." FORCE)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/out/bin/${CMAKE_BUILD_TYPE} CACHE
    PATH "Directory where .exe and .dll files are dumped." FORCE)
SET(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake CACHE
    PATH "Directory where .cmake files are stored." FORCE)

