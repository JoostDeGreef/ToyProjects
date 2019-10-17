# CMakeList.txt : CMake project for MathCore, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

link_libraries(SQLite)

# Add source to this project's executable.
add_library (SQLiteDB SHARED 
             "src/SQLiteDB.cpp"
             "src/SQLiteStatement.cpp"
			 "src/SQLiteQuery.cpp")

