add_library(calabash_thread
        OBJECT
        thread_pool.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_thread>
        PARENT_SCOPE)