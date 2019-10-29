include(Common)

include(ExternalProject)

SET(GLFW_DIR ${PROJECT_SOURCE_DIR}/out/3rdParty-build/glfw/${CMAKE_BUILD_TYPE})

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)

ExternalProject_Add(glfw
  EXCLUDE_FROM_ALL 1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw"
  CMAKE_CACHE_ARGS
         -DCMAKE_INSTALL_PREFIX:PATH=${GLFW_DIR}
         -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
         -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=true
)

SET(GLFW_HEADER_FOLDER ${GLFW_DIR}/include)
if(UNIX)
   SET(GLFW_FILE ${GLFW_DIR}/lib/libglfw.a)
else()
   SET(GLFW_FILE ${GLFW_DIR}/lib/glfw.lib)
endif()

# how to build the result of the library
add_custom_command(OUTPUT ${GLFW_FILE}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
                   DEPENDS glfw)
# create a target out of the library compilation result
add_custom_target(glfw_target DEPENDS ${GLFW_FILE})

# create a library target out of the library compilation result
add_library(GLFW INTERFACE)
add_dependencies(GLFW glfw_target)
target_include_directories(GLFW INTERFACE ${GLFW_HEADER_FOLDER})
target_link_libraries(GLFW INTERFACE ${GLFW_FILE})

