cmake_minimum_required(VERSION 3.0.0)

project(openglapp)

set (CMAKE_CXX_STANDARD 14)

find_package(glfw3)
find_package(glew)

file(GLOB_RECURSE SRCS "src" *.cpp *.hpp)

add_executable(openglapp ${SRCS})

target_link_libraries(openglapp glfw glew)
