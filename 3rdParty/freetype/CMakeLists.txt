include(Common)

include(ExternalProject)

SET(FREETYPE_DIR ${PROJECT_SOURCE_DIR}/out/3rdParty-build/freetype/${CMAKE_BUILD_TYPE})

ExternalProject_Add(freetype
  EXCLUDE_FROM_ALL 1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/freetype"
  CMAKE_CACHE_ARGS
         -DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_DIR}
         -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
         -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=true
		 -DSKIP_INSTALL_HEADERS:BOOL=true
		 -DDISABLE_FORCE_DEBUG_POSTFIX:BOOL=true
)

SET(FREETYPE_HEADER_FOLDER freetype/include)
if(UNIX)
   SET(FREETYPE_FILE ${FREETYPE_DIR}/lib/libfreetype.a)
else()
   SET(FREETYPE_FILE ${FREETYPE_DIR}/lib/freetype.lib)
endif()

# how to build the result of the library
add_custom_command(OUTPUT ${FREETYPE_FILE}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
                   DEPENDS freetype)
# create a target out of the library compilation result
add_custom_target(freetype_target DEPENDS ${FREETYPE_FILE})

# create a library target out of the library compilation result
add_library(FREETYPE INTERFACE)
add_dependencies(FREETYPE freetype_target)
target_include_directories(FREETYPE INTERFACE ${FREETYPE_HEADER_FOLDER})
target_link_libraries(FREETYPE INTERFACE ${FREETYPE_FILE})

