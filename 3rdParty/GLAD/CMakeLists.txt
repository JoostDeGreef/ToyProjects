include(Common)

PROJECT(GLAD)

include_directories("glad/include")

# Add source to this project's executable.
add_library (${PROJECT_NAME}
             "glad/src/glad.c")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "glad/include"
)
