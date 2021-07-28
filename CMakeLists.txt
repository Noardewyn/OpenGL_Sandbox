cmake_minimum_required (VERSION 3.8)

# 3rd party libs
add_subdirectory(vendor/glfw)
include_directories(vendor/glfw/include)
include_directories(vendor/glew-2.1.0/include)
include_directories(vendor/glm)
include_directories("C:/Users/alexey/OneDrive/SOIL2/src")

link_directories("vendor/glew-2.1.0/lib")
link_directories("C:/Users/alexey/OneDrive/lib")

# project modules
add_subdirectory ("Render")
add_subdirectory ("Sandbox")
