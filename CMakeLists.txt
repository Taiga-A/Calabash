cmake_minimum_required(VERSION 3.15)
project(Calabash)

set(CMAKE_CXX_STANDARD 17)

set(CALABASH_INCLUDE
        ${PROJECT_SOURCE_DIR}/src/include
        ${PROJECT_SOURCE_DIR}/third_party/sqlite3/include
        ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
        ${PROJECT_SOURCE_DIR}/third_party/nlohmann_json/single_include)

include_directories(${CALABASH_INCLUDE})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/out/lib)

add_subdirectory(src)
add_subdirectory(third_party)
add_subdirectory(test)
add_subdirectory(example)