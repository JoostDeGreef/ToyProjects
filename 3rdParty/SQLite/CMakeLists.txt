include(Common)

include(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteVersion.cmake)
project(SQLite VERSION ${SQLITE_VERSION} DESCRIPTION "SQLite database engine")

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library (${PROJECT_NAME}
             "sqlite/sqlite3.c")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
