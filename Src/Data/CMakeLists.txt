include(Common)

project(Data VERSION 1.0.0 DESCRIPTION "Data loader functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

link_libraries(SQLiteDB
               boost
			   Boost-FileSystem)

add_library (${PROJECT_NAME} SHARED
             "src/Data.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

