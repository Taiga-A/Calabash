add_library(calabash_socket
        OBJECT
        event_poller.cpp
        server_socket.cpp
        socket.cpp
        socket_handler.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:calabash_socket>
        PARENT_SCOPE)
