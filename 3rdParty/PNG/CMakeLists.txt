include(Common)

project(PNG)

include(ExternalProject)

set(PNG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/png/${CMAKE_BUILD_TYPE})

ExternalProject_Add(png
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code"
  CMAKE_CACHE_ARGS
         -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/lib/${CMAKE_BUILD_TYPE}
         -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}
         -DCMAKE_INSTALL_PREFIX:PATH=${PNG_DIR}
         PNG_BUILD_ZLIB=ON
)

set(PNG_FILE ${PNG_DIR}/lib/pngstatic)
set(PNG_HEADER_FOLDER ${PNG_DIR}/include)

# how to build the result of the library
#add_custom_command(OUTPUT  ${LIB_FILE}
#                   COMMAND make 
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# create a target out of the library compilation result
add_custom_target(png_target DEPENDS ${PNG_FILE})

# create an library target out of the library compilation result
add_library(PNG STATIC IMPORTED GLOBAL)
add_dependencies(PNG png_target)

# specify where the library is and where to find the headers
set_target_properties(PNG
    PROPERTIES
    IMPORTED_LOCATION ${PNG_FILE}
    INTERFACE_INCLUDE_DIRECTORIES ${PNG_HEADER_FOLDER})


#option(PNG_BUILD_ZLIB ON)
#option(PNG_SHARED ON)
#option(PNG_STATIC ON)
#option(PNG_TESTS  OFF)

#set(ZLIB_LIBRARY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/zlibstatic.lib)
#set(ZLIB_INCLUDE_DIR "../../include/ZLIB")

#add_subdirectory ("code")


