include(Common)

project(Data VERSION 1.0.0 DESCRIPTION "Data loader functions")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

link_libraries(SQLiteDB
               Utils
               boost
               Boost-FileSystem)

add_library (${PROJECT_NAME} # SHARED
             "src/Data.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data/processed/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying data files...")

