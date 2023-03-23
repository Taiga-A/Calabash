add_library(calabash_app
        OBJECT
        student_mange_server.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_app>
        PARENT_SCOPE)