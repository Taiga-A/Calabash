#add_subdirectory(app)
add_subdirectory(database)
add_subdirectory(json)
add_subdirectory(socket)
add_subdirectory(thread)
add_subdirectory(util)
add_subdirectory(web)

add_library(calabash
        STATIC
        ${ALL_OBJECT_FILES})

set(CALABASH_ALL_LIBS
        calabash_json
        calabash_socket
        calabash_thread
        calabash_util
        calabash_web)

set(CALABASH_THIRD_LIBS
        pthread
        sqlite3)

target_link_libraries(calabash
        ${CALABASH_ALL_LIBS}
        ${CALABASH_THIRD_LIBS})

target_include_directories(
        calabash PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)