add_library(calabash_database
        OBJECT
        sqlite3.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_database>
        PARENT_SCOPE)