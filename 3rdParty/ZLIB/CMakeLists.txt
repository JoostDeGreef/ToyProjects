include(Common)

project(ZLIB)

include(ExternalProject)

SET(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/zlib/${CMAKE_BUILD_TYPE})

# Builds c-ares project from the git submodule.
# Note: For all external projects, instead of using checked-out code, one could
# specify GIT_REPOSITORY and GIT_TAG to have cmake download the dependency directly,
# without needing to add a submodule to your project.
ExternalProject_Add(zlib
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib"
  CMAKE_CACHE_ARGS
         -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/lib/${CMAKE_BUILD_TYPE}
         -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}
         -DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_DIR}
)

SET(ZLIB_HEADER_FOLDER ${ZLIB_DIR}/include)
if(UNIX)
   SET(ZLIB_FILE ${ZLIB_DIR}/lib/libz.a)
else
   SET(ZLIB_FILE ${ZLIB_DIR}/lib/zlib.lib)
endif()

# how to build the result of the library
#add_custom_command(OUTPUT  ${LIB_FILE}
#                   COMMAND make 
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# create a target out of the library compilation result
#add_custom_target(zlib_target DEPENDS ${ZLIB_FILE})
add_custom_target(zlib_target DEPENDS zlib)

# create an library target out of the library compilation result
add_library(ZLIB STATIC IMPORTED GLOBAL)
add_dependencies(ZLIB zlib_target)

# specify where the library is and where to find the headers
set_target_properties(ZLIB
    PROPERTIES
    IMPORTED_LOCATION ${ZLIB_FILE}
    INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_HEADER_FOLDER})


