add_library(calabash_util
        OBJECT
        logger.cpp
        data_reader.cpp
        md5.cpp
        base64.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_util>
        PARENT_SCOPE)
