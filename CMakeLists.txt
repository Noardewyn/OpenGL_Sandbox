# CMakeList.txt: проект CMake для OpenGL_Render; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("OpenGL_Render")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (OpenGL_Render "OpenGL_Render.cpp" "OpenGL_Render.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
