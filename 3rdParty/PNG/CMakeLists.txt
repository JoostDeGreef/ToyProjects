include(Common)

include(ExternalProject)

SET(PNG_DIR ${PROJECT_SOURCE_DIR}/out/3rdParty-build/png/${CMAKE_BUILD_TYPE})

# this is ugly.
SET(ZLIB_DIR ${PROJECT_SOURCE_DIR}/out/3rdParty-build/zlib/${CMAKE_BUILD_TYPE})
SET(ZLIB_HEADER_FOLDER ${ZLIB_DIR}/include)
if(UNIX)
   SET(ZLIB_FILE ${ZLIB_DIR}/lib/libz.a)
else()
   SET(ZLIB_FILE ${ZLIB_DIR}/lib/zlibstatic.lib)
endif()

ExternalProject_Add(png
  EXCLUDE_FROM_ALL 1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code"
  DEPENDS ZLIB
  CMAKE_CACHE_ARGS
         -DCMAKE_INSTALL_PREFIX:PATH=${PNG_DIR}
         -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
         -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=true
         -DPNG_BUILD_ZLIB:BOOL=true
		 -DZLIB_LIBRARY:PATH=${ZLIB_FILE}
		 -DZLIB_INCLUDE_DIR:PATH=${ZLIB_HEADER_FOLDER}
         -DPNG_SHARED:BOOL=false
         -DPNG_STATIC:BOOL=true
         -DPNG_TESTS:BOOL=true
)

SET(PNG_HEADER_FOLDER ${PNG_DIR}/include)
IF( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	if(UNIX)
	   SET(PNG_FILE ${PNG_DIR}/lib/libpng16d.a)
	else()
	   SET(PNG_FILE ${PNG_DIR}/lib/libpng16_staticd.lib)
	endif()
ELSE()
	if(UNIX)
	   SET(PNG_FILE ${PNG_DIR}/lib/libpng16.a)
	else()
	   SET(PNG_FILE ${PNG_DIR}/lib/libpng16_static.lib)
	endif()
ENDIF()

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
