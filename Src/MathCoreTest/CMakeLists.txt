# CMakeList.txt : CMake project for MathCore, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    link_libraries(GoogleTest Threads::Threads)
endif(CMAKE_COMPILER_IS_GNUCC)

link_libraries(GoogleTest 
               MathCore)

# Add source to this project's executable.
add_executable (MathCoreTest
                "MathCoreTest.cpp")

# TODO: Add tests and install targets if needed.
