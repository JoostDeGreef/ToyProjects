include(Common)

include(ExternalProject)

SET(ZLIB_DIR ${PROJECT_SOURCE_DIR}/out/3rdParty-build/zlib/${CMAKE_BUILD_TYPE})

ExternalProject_Add(zlib
  EXCLUDE_FROM_ALL 1
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib"
  CMAKE_CACHE_ARGS
         -DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_DIR}
         -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
         -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=true
)

SET(ZLIB_HEADER_FOLDER ${ZLIB_DIR}/include)
if(UNIX)
   SET(ZLIB_FILE ${ZLIB_DIR}/lib/libz.a)
else()
   SET(ZLIB_FILE ${ZLIB_DIR}/lib/zlib.lib)
endif()

# how to build the result of the library
add_custom_command(OUTPUT ${ZLIB_FILE}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
                   DEPENDS zlib)
# create a target out of the library compilation result
add_custom_target(zlib_target DEPENDS ${ZLIB_FILE})

# create a library target out of the library compilation result
add_library(ZLIB INTERFACE)
add_dependencies(ZLIB zlib_target)
target_include_directories(ZLIB INTERFACE ${ZLIB_HEADER_FOLDER})
target_link_libraries(ZLIB INTERFACE ${ZLIB_FILE})

