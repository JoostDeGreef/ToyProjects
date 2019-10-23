project(Boost-FileSystem)

#set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

#include(Library)

file(GLOB SRC_FILES ../../boost/libs/filesystem/src/*.cpp)

add_library (${PROJECT_NAME} ${SRC_FILES})

