cmake_minimum_required (VERSION 3.8)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB SRC_FILES ../../boost/libs/filesystem/src/*.cpp)

add_library (Boost-FileSystem ${SRC_FILES})

