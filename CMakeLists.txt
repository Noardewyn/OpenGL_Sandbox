cmake_minimum_required (VERSION 3.8)
project(OpenGL_Render)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (MSVC)
    add_compile_options(/EHsc)
endif()

 set(CMAKE_MODULE_PATH ${_install_dir})

find_package(glfw3)
find_package(glew)
find_package(glm)
find_package(stb)
find_package(spdlog)
find_package(imgui)
find_package(assimp)

include_directories("./bindings")

# project modules
add_subdirectory ("Render")
add_subdirectory ("Sandbox")
