include(Common)

include(ExternalProject)

SET(PNG_DIR ${PROJECT_SOURCE_DIR}/out/3rdParty-build/png/${CMAKE_BUILD_TYPE})

ExternalProject_Add(png
  EXCLUDE_FROM_ALL 1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code"
  CMAKE_CACHE_ARGS
         -DCMAKE_INSTALL_PREFIX:PATH=${PNG_DIR}
         -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
         -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=true
         PNG_BUILD_ZLIB=ON
)

#option(PNG_BUILD_ZLIB ON)
#option(PNG_SHARED OFF)
#option(PNG_STATIC ON)
#option(PNG_TESTS  OFF)

SET(PNG_HEADER_FOLDER ${PNG_DIR}/include)
if(UNIX)
   SET(PNG_FILE ${PNG_DIR}/lib/libpng.a)
else()
   SET(PNG_FILE ${PNG_DIR}/lib/libpng.lib)
endif()

# how to build the result of the library
add_custom_command(OUTPUT ${PNG_FILE}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
                   DEPENDS png)
# create a target out of the library compilation result
add_custom_target(png_target DEPENDS ${PNG_FILE})

# create a library target out of the library compilation result
add_library(PNG INTERFACE)
add_dependencies(PNG png_target ZLIB)
target_include_directories(PNG INTERFACE ${PNG_HEADER_FOLDER})
target_link_libraries(PNG INTERFACE ${PNG_FILE})
