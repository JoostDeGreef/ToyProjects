include(Library)

project(MathCore)

# Why doesn't SHARED work?
add_library (${PROJECT_NAME}  
             "src/MathCore.cpp")

# directories where to search for header files
target_include_directories(${PROJECT_NAME} PUBLIC include)

