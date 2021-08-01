cmake_minimum_required (VERSION 3.8)
project(OpenGL_Render)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CONAN_COMPILER_VERSION 16)

# Automatic get dependencies with conan package manager
# https://docs.conan.io/en/latest/introduction.html
include(cmake/conan_get_dependencies.cmake)

conan_get_dependencies(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(spdlog MODULE REQUIRED)
find_package(glfw3 MODULE REQUIRED)
find_package(glm MODULE REQUIRED)
find_package(GLEW MODULE REQUIRED)
find_package(imgui MODULE REQUIRED)

include_directories("./bindings")

# project modules
add_subdirectory ("Render")
add_subdirectory ("Sandbox")
