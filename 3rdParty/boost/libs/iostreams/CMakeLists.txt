project(Boost-IOStreams)

#set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

#include(Library)

file(GLOB SRC_FILES ../../boost/libs/iostreams/src/zlib.cpp)

add_library (${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ZLIB)
