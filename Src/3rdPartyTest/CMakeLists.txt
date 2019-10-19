include(Executable)

link_libraries(GoogleTest 
               Boost-FileSystem)

add_executable (3rdPartyTest
                "GoogleTest/OutputTest.cpp"
                "Boost/FileSystemTest.cpp")

