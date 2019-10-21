include(Library)

include_directories("glad/include")

# Add source to this project's executable.
add_library (GLAD
             "glad/src/glad.c")

