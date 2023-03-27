add_library(calabash_web
        OBJECT
        exception.cpp
        http_task.cpp
        request.cpp
        response.cpp
        server.cpp
        request_maker.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_web>
        PARENT_SCOPE)