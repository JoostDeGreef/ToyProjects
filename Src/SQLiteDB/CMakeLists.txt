include(Library)

link_libraries(SQLite)

project(SQLiteDB)

# Add source to this project's executable.
add_library (${PROJECT_NAME} SHARED 
             "src/SQLiteDB.cpp"
             "src/SQLiteStatement.cpp"
			 "src/SQLiteQuery.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

