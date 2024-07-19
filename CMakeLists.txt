cmake_minimum_required(VERSION 3.8)
project(GDMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WIN32_EXECUTABLE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

execute_process(
    COMMAND python3 packages.py
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    TIMEOUT 60
)

if (result EQUAL 0)
    message(STATUS "Command succeeded: ${output}")
else()
    message(FATAL_ERROR "Command failed with error code ${result}: ${error}")
endif()



add_subdirectory(src)
add_subdirectory(src/actions)
add_subdirectory(src/animations)
add_subdirectory(src/core)
add_subdirectory(src/base)
add_subdirectory(src/editor)
add_subdirectory(src/gui)
add_subdirectory(src/display)


file(GLOB_RECURSE ALL_SOURCE_FILES "src/actions/*.c")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/animations/*.c")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/core/*.c")




