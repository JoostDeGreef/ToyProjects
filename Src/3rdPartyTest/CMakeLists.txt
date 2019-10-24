include(Common)

project(3rdPartyTest)

link_libraries(GoogleTest 
               boost
               Boost-FileSystem)

add_executable (${PROJECT_NAME}
                "GoogleTest/OutputTest.cpp"
                "Boost/FileSystemTest.cpp")

