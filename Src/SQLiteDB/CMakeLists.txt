cmake_minimum_required(VERSION 3.9)

project(SQLiteDB VERSION 1.0.0 DESCRIPTION "C++ wrapper for SQLite")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#project(SQLiteDB)

#include(Library)

link_libraries(SQLite
               boost)

# Add source to this project's executable.
add_library (${PROJECT_NAME} SHARED 
             "src/SQLiteDB.cpp"
             "src/SQLiteStatement.cpp"
			 "src/SQLiteQuery.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

