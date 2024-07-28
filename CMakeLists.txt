cmake_minimum_required(VERSION 3.8)
project(GDMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WIN32_EXECUTABLE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")





# Main 
file(GLOB_RECURSE MAIN_SOURCE_FILES "main/*.cpp" )

# CORE
file(GLOB_RECURSE CONFIG_SOURCE_FILES "core/config/*.cpp")
file(GLOB_RECURSE DEBUG_SOURCE_FILES "core/debug/*.cpp")
file(GLOB_RECURSE ERROR_SOURCE_FILES "core/error/*.cpp")
file(GLOB_RECURSE OBJECT_SOURCE_FILES "core/object/*.cpp")
file(GLOB_RECURSE INPUT_SOURCE_FILES "core/input/*.cpp")
file(GLOB_RECURSE EXTENTION_SOURCE_FILES "core/extentions/*.cpp")
file(GLOB_RECURSE MATH_SOURCE_FILES "core/math/*.cpp")
file(GLOB_RECURSE DATA_SOURCE_FILES "core/data/*.cpp")
file(GLOB_RECURSE STD_SOURCE_FILES "core/std/*.cpp")
file(GLOB_RECURSE OS_SOURCE_FILES "core/os/*.cpp")
file(GLOB_RECURSE IO_SOURCE_FILES "core/io/*.cpp")
file(GLOB_RECURSE STRING_SOURCE_FILES "core/string/*.cpp")
file(GLOB_RECURSE VARIANT_SOURCE_FILES "core/variant/*.cpp")


# BASE
file(GLOB_RECURSE BASE_SOURCE_FILES "base/config/*.cpp")


# EDITOR
file(GLOB_RECURSE EDITOR_SOURCE_FILES "editor/*.cpp")
file(GLOB_RECURSE EDITOR_SOURCE_FILES "editor/gui/*.cpp")
file(GLOB_RECURSE EDITOR_SOURCE_FILES "editor/themes/*.cpp")
file(GLOB_RECURSE EDITOR_SOURCE_FILES "editor/debug/*.cpp")
file(GLOB_RECURSE EDITOR_SOURCE_FILES "editor/export/*.cpp")


# SERVERS
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/renderer/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/storage/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/texture/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/display/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/scale/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/time/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "servers/shaders/*.cpp")


# SCENE
file(GLOB_RECURSE SCENE_SOURCE_FILES "scene/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/2d/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/3d/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/gui/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/animations/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/debug/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/audio/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/main/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/resource/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/pyshics/2d/*.cpp")
file(GLOB_RECURSE ALL_SOURCE_FILES "scene/pyshics/3d/*.cpp")


# CACHE
file(GLOB_RECURSE CACHE_SOURCE_FILES "cache/*.cpp")

# PLATFORM
file(GLOB_RECURSE PLATFORM_SOURCE_FILES "platform/*.cpp")

# MODULES
file(GLOB_RECURSE CAMERA_SOURCE_FILES "modules/camara/*.cpp")
file(GLOB_RECURSE PNG_SOURCE_FILES "modules/png/*.cpp")
file(GLOB_RECURSE OGG_SOURCE_FILES "modules/ogg/*.cpp")
file(GLOB_RECURSE MULTIPLAYER_SOURCE_FILES "modules/multiplayer/*.cpp")


# THIEDPARTY LIBARIES
file(GLOB_RECURSE TOML_SOURCE_FILES "thirdparty/toml11/*.cpp")
file(GLOB_RECURSE PUGIXML_SOURCE_FILES "thirdparty/pugixml/*.cpp")
file(GLOB_RECURCE RLIMGUI_SOURCE_FILES "thirdparty/rlImGui/*.cpp")
file(GLOB_RECURCE IMGUI_SOURCE_FILES "thirdparty/imgui/*.cpp")






