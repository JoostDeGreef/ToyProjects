include(Common)

project(GoogleTest VERSION 1.0.0 DESCRIPTION "Google test framework")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library (${PROJECT_NAME} SHARED 
             "src/gtest/gtest-all.cc"
             "src-extra/ConsoleOutput.cpp"
             "src-extra/ConsoleOutput.h"
             "src-extra/GoogleTestMain.cpp"
             "src-extra/VisualStudioOutput.cpp"
             "src-extra/VisualStudioOutput.h")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCC)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} INTERFACE Threads::Threads)
endif(UNIX)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src-extra
)

