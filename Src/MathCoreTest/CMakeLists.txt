cmake_minimum_required (VERSION 3.8)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    link_libraries(Threads::Threads)
endif(CMAKE_COMPILER_IS_GNUCC)

project(MathCoreTest)

#find_package(ToyProjects::MathCore REQUIRED)

link_libraries(GoogleTest
               MathCore)

add_executable (${PROJECT_NAME}
                "VectorTest.cpp"
				"QuaternionTest.cpp"
                "MathCoreTest.cpp")

