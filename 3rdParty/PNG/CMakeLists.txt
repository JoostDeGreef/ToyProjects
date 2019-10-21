option(PNG_BUILD_ZLIB ON)
option(PNG_SHARED ON)
option(PNG_STATIC ON)
option(PNG_TESTS  OFF)

set(ZLIB_LIBRARY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/zlibstatic.lib)
set(ZLIB_INCLUDE_DIR "../../include/ZLIB")

add_subdirectory ("code")

#add_library(GLFW ALIAS glfw)
