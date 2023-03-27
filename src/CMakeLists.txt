add_subdirectory(database)
add_subdirectory(socket)
add_subdirectory(thread)
add_subdirectory(util)
add_subdirectory(web)

add_library(calabash
        STATIC
        ${ALL_OBJECT_FILES})

set(CALABASH_ALL_LIBS
        calabash_socket
        calabash_thread
        calabash_util
        calabash_web
        )

set(CALABASH_THIRD_LIBS
        pthread
        sqlite3
        nlohmann_json::nlohmann_json)

target_link_libraries(calabash
        ${CALABASH_ALL_LIBS}
        ${CALABASH_THIRD_LIBS})

target_include_directories(
        calabash PUBLIC
        ${CALABASH_INCLUDE})

set(COPY_INCLUDE_DIR "$<TARGET_FILE_DIR:calabash>/../include/calabash")
add_custom_command(TARGET calabash POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${COPY_INCLUDE_DIR})

foreach(INCLUDE_PATH ${CALABASH_INCLUDE})
    add_custom_command(TARGET calabash POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${INCLUDE_PATH}"
            "${COPY_INCLUDE_DIR}")
endforeach()