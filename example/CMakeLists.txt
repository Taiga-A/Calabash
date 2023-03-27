include_directories(include)

add_subdirectory(app)

add_executable(server server.cpp $<TARGET_OBJECTS:template_app>)

target_link_libraries(server calabash)
