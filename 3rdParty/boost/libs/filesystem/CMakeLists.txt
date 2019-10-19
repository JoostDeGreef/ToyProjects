include(Library)

file(GLOB SRC_FILES ../../boost/libs/filesystem/src/*.cpp)

add_library (Boost-FileSystem ${SRC_FILES})

