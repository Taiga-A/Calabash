add_library(calabash_util
        OBJECT
        logger.cpp
        #        str.cpp
        data_reader.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_util>
        PARENT_SCOPE)
