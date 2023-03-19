
file(GLOB_RECURSE CALABASH_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

foreach (calabash_test_source ${CALABASH_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(calabash_test_filename ${calabash_test_source} NAME)

    string(REPLACE ".cpp" "" calabash_test_name ${calabash_test_filename})

    add_executable(${calabash_test_name} EXCLUDE_FROM_ALL ${calabash_test_source})

    target_link_libraries(${calabash_test_name} calabash gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${calabash_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${calabash_test_name}
            )
endforeach ()