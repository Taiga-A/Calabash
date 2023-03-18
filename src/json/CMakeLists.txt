add_library(calabash_json
        OBJECT
        json.cpp
        parser.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_json>
        PARENT_SCOPE)