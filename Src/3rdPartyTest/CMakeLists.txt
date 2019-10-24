include(Common)

project(3rdPartyTest)

link_libraries(GoogleTest
               boost
               Boost-FileSystem
               Boost-IOStreams)

add_executable (${PROJECT_NAME}
                "GoogleTest/OutputTest.cpp"
                "Boost/FileSystemTest.cpp"
                "Boost/IOStreamsTest.cpp")

