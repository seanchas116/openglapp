cmake_minimum_required(VERSION 3.11.1)

project(openglapp)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)

# Tell CMake to create the helloworld executable
add_executable(openglapp src/main.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(openglapp Qt5::Widgets Qt5::OpenGL)
