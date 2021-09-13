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
    
    # Engine sources
    ${PROJECT_SOURCE_DIR}/engine/src/BaseScene.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/Scene.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/Entity.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/Component.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/Material.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/Model.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/MeshProvider.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/assets/Asset.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/assets/ShaderAsset.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/assets/AssetManager.cpp

    # Components
    ${PROJECT_SOURCE_DIR}/engine/src/components/MeshRenderer.cpp
    ${PROJECT_SOURCE_DIR}/engine/src/components/Light.cpp
    
    # Sandbox sources
    ${PROJECT_SOURCE_DIR}/examples/ExamplesMenu.cpp
    ${PROJECT_SOURCE_DIR}/examples/ClearColorExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/ColoredQuadExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/TextureExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/FragmentCircleExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/Camera3DExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/Lighting3DExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/AdvancedLighting3DExample.cpp
    ${PROJECT_SOURCE_DIR}/examples/SceneGraphExample.cpp
#    ${PROJECT_SOURCE_DIR}/examples/StencilExample.cpp
#    ${PROJECT_SOURCE_DIR}/examples/FramebufferExample.cpp
    
    ${PROJECT_SOURCE_DIR}/main.cpp
)

set(LIBS
    Render 
    imgui::imgui
    assimp::assimp
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/engine/include
)
