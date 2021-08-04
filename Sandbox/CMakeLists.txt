cmake_minimum_required (VERSION 3.8)
project ("Sandbox")

message(STATUS "-----> Current Project: ${PROJECT_NAME}")
message(STATUS "-----> Current SOURCE directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "-----> Current BUILD directory: ${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_CXX_FLAGS "-DIMGUI_IMPL_OPENGL_LOADER_GLEW")

set(HEADERS
    ${CMAKE_SOURCE_DIR}/bindings/imgui_impl_glfw.h
    ${CMAKE_SOURCE_DIR}/bindings/imgui_impl_opengl3.h
)

set(SOURCES
    # imgui sources
    ${CMAKE_SOURCE_DIR}/bindings/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/bindings/imgui_impl_glfw.cpp
    # Sandbox sources
    ${PROJECT_SOURCE_DIR}/examples/ExamplesMenu.cpp
    ${PROJECT_SOURCE_DIR}/examples/ClearColorExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/ColoredQuadExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/TextureExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/FragmentCircleExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/Camera3DExample.cpp

    ${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Render imgui::imgui)

# Copy assets to project binary dir
add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/assets/
                ${CMAKE_BINARY_DIR}/bin/assets/)