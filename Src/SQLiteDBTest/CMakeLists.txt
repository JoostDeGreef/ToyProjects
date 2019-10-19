include(Executable)

project(SQLiteTest)

link_libraries(GoogleTest 
			   SQLiteDB)

add_executable (${PROJECT_NAME}
				"SQLiteTest.cpp")

