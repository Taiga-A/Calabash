include_directories(include)

add_library(sqlite3 OBJECT sqlite3.c)

target_link_libraries(sqlite3 dl)