cmake_minimum_required(VERSION 3.0.0)

project(openglapp)

find_package(glfw3)
find_package(glew)

add_executable(openglapp src/main.cpp)

target_link_libraries(openglapp glfw glew)
