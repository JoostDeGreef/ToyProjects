
cmake_minimum_required(VERSION 3.9)

project(SQLite VERSION 1.0.0 DESCRIPTION "SQLite database")

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library (${PROJECT_NAME}
           # SHARED
             "sqlite/sqlite3.c")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)

