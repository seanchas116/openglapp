cmake_minimum_required(VERSION 3.0.0)

project(openglapp)

set (CMAKE_CXX_STANDARD 14)

find_package(OpenGL)
find_package(glfw3)
find_package(glew)
include_directories("vendor/stb")
include_directories("vendor/tinyobjloader")

file(GLOB_RECURSE SRCS src/*.cpp src/*.hpp)
file(GLOB_RECURSE SHADERS resources/*.vert resources/*.frag)

add_executable(openglapp ${SRCS} ${SHADERS} vendor/tinyobjloader/tiny_obj_loader.cc)

target_link_libraries(openglapp ${OPENGL_LIBRARY} glfw glew)
