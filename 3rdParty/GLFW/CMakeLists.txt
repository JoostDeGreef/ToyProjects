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
   SET(GLFW_FILE ${GLFW_DIR}/lib/libglfw3.a)
else()
   SET(GLFW_FILE ${GLFW_DIR}/lib/glfw3.lib)
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
if(UNIX)
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    if(NOT X11_Xrandr_FOUND)
       message(FATAL_ERROR "Xrandr library not found - required for GLFW")
    endif()
    if(NOT X11_xf86vmode_FOUND)
       message(FATAL_ERROR "xf86vmode library not found - required for GLFW")
    endif()
    if(NOT X11_Xcursor_FOUND)
       message(FATAL_ERROR "Xcursor library not found - required for GLFW")
    endif()
    if(NOT X11_Xinerama_FOUND)
      message(FATAL_ERROR "Xinerama library not found - required for GLFW")
    endif()
    if(NOT X11_Xi_FOUND)
      message(FATAL_ERROR "Xi library not found - required for GLFW")
    endif()
    target_link_libraries(GLFW INTERFACE ${GLFW_FILE} ${CMAKE_DL_LIBS} Threads::Threads "${X11_Xrandr_LIB}" "${X11_Xxf86vm_LIB}" "${X11_Xcursor_LIB}" "${X11_Xinerama_LIB}" "${X11_Xi_LIB}" "${X11_LIBRARIES}")
else()
    target_link_libraries(GLFW INTERFACE ${GLFW_FILE} ${CMAKE_DL_LIBS})
endif(UNIX)


