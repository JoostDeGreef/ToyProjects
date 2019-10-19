include(Library)

project(MathCore)

add_library (${PROJECT_NAME} SHARED 
             "src/MathCore.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

