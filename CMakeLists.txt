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
execute_process(
 12     COMMAND python3 libaries.py
 13     RESULT_VARIABLE result
 14     OUTPUT_VARIABLE output
 15     ERROR_VARIABLE error
 16     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
 17     TIMEOUT 60
 18 )

if (result EQUAL 0)
    message(STATUS "Command succeeded: ${output}")
else()
    message(FATAL_ERROR "Command failed with error code ${result}: ${error}")
endif()

add_subdirectory(tools)
add_subdirectory(modules)
add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(src/actions)
add_subdirectory(src/animations)
add_subdirectory(src/core)
add_subdirectory(src/base)
add_subdirectory(src/editor)
add_subdirectory(src/gui)
add_subdirectory(src/display)
add_subdirectory(src/time)
add_subdirectory(src/scene)
add_subdirectory(src/server)
add_subdirectory(src/tilemap)
add_subdirectory(src/utils)
add_subdirectory(src/types)



file(GLOB_RECURSE ALL_SOURCE_FILES "src/actions/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/animations/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/core/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/base/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/editor/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/display/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/gui/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/server/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/scene/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/utils/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "src/tilemaps/*.cpp")






